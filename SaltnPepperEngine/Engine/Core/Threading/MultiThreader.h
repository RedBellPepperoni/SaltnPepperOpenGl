#ifndef MULTITHREADER_H
#define MULTITHREADER_H

#include <functional>

namespace SaltnPepperEngine
{

	struct ThreadingArgs
	{
		uint32_t jobIndex;
		uint32_t groupIndex;
	};

	namespace MultiThreader
	{
		// Creates all the base setup for the Multi threader
		void Init();

		// Add a work thrad to work on a task, without syncing with others
		//Use ful to do different unrealted stuff at the same time
		void ParallelTask(const std::function<void()>& task);

		// Divide a task into sub task and execute in parallel
		void ThreadedTask(uint32_t jobCount, uint32_t groupSize, const std::function<void(ThreadingArgs)>& task);

		// Chech if the thread is curretly in use
		bool IsBusy();

		// Wait till all the threads are finished working
		void Wait();
	}
}

#endif // !MULTITHREADER_H
