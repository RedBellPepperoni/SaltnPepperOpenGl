#include "MultiThreader.h"

#include <algorithm>    
#include <atomic>   
#include <thread>    
#include <condition_variable>    
#include <sstream>
#include <assert.h>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

// Fixed size very simple thread safe ring buffer
template <typename T, size_t capacity>
class ThreadSafeRingBuffer
{
public:
	// Push an item to the end if there is free space
	inline bool push_back(const T& item)
	{
		bool result = false;
		lock.lock();
		size_t next = (head + 1) % capacity;
		if (next != tail)
		{
			data[head] = item;
			head = next;
			result = true;
		}
		lock.unlock();
		return result;
	}

	// Get an item if there are any
	inline bool pop_front(T& item)
	{
		bool result = false;
		lock.lock();
		if (tail != head)
		{
			item = data[tail];
			tail = (tail + 1) % capacity;
			result = true;
		}
		lock.unlock();
		return result;
	}

private:
	T data[capacity];
	size_t head = 0;
	size_t tail = 0;
	std::mutex lock;
};


namespace SaltnPepperEngine
{
	namespace MultiThreader
	{
		// The toal number of threads, to use
		uint32_t numThreads = 0;

		// a queue to store pending jobs
		ThreadSafeRingBuffer<std::function<void()>, 256> taskPool;

		// used in conjunction with the Mutex. Main thread can wake up idle workers
		std::condition_variable wakeCondition;
		std::mutex wakeMutex;

		// Tracks the executon of the main thread
		uint64_t currentLabel = 0;

		// Tracks execution of worker threads
		std::atomic<uint64_t> finishedLabel;


		void Init()
		{

			finishedLabel.store(0);

			// Get the number of actual hardware threads
			auto numCores = std::thread::hardware_concurrency();
			numThreads = std::max(1u, numCores);

			//Create all the worker threads
			for (uint32_t threadID = 0; threadID < numThreads; ++threadID)
			{
				std::thread worker([] {

					// empty job
					std::function<void()> task;

					//Looping call for adding new task to workers
					while (true)
					{
						// get a job from the threadsafe queue
						if (taskPool.pop_front(task))
						{
							// Execute the task
							task();
							finishedLabel.fetch_add(1);
						}
						else
						{
							// no task, sleep
							std::unique_lock<std::mutex> lock(wakeMutex);
							wakeCondition.wait(lock);
						}
					}

					});



				HANDLE handle = (HANDLE)worker.native_handle();

				// Adding a thread for its dedicated core
				DWORD_PTR affinityMask = 1ull << threadID;
				DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
				assert(affinity_result > 0);

				// Name the thread:
				std::wstringstream wss;
				wss << "MultiThreader" << threadID;
				HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
				assert(SUCCEEDED(hr));

				worker.detach();
			}
		}

		//helper function to prevent Deadlock
		inline void poll()
		{
			//Wake up a thread
			wakeCondition.notify_one();
			// Rescehdule it
			std::this_thread::yield();
		}

		void ParallelTask(const std::function<void()>& task)
		{
			// The main thread label state is updated:
			currentLabel += 1;

			// Try to push a new job until it is pushed successfully:
			while (!taskPool.push_back(task)) { poll(); }

			wakeCondition.notify_one(); // wake one thread
		}

		bool IsBusy()
		{
			// Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
			return finishedLabel.load() < currentLabel;
		}

		void Wait()
		{
			while (IsBusy()) { poll(); }
		}

		void ThreadedTask(uint32_t jobCount, uint32_t groupSize, const std::function<void(ThreadingArgs)>& task)
		{
			if (jobCount == 0 || groupSize == 0)
			{
				return;
			}

			// Calculate the amount of job groups to dispatch (overestimate, or "ceil"):
			const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

			// The main thread label state is updated:
			currentLabel += groupCount;

			for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex)
			{
				// For each group, generate one real job:
				const auto& jobGroup = [jobCount, groupSize, task, groupIndex]() {

					// Calculate the current group's offset into the jobs:
					const uint32_t groupJobOffset = groupIndex * groupSize;
					const uint32_t groupJobEnd = std::min(groupJobOffset + groupSize, jobCount);

					ThreadingArgs args;
					args.groupIndex = groupIndex;

					// Inside the group, loop through all job indices and execute job for each index:
					for (uint32_t i = groupJobOffset; i < groupJobEnd; ++i)
					{
						args.jobIndex = i;
						task(args);
					}
					};

				// Try to push a new job until it is pushed successfully:
				while (!taskPool.push_back(jobGroup)) { poll(); }

				wakeCondition.notify_one(); // wake one thread
			}
		}
	}
}