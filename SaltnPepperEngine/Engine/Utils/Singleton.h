#ifndef SINGLETON_H
#define SINGLETON_H
#include <mutex>

namespace SaltnPepperEngine
{
    template<class T>
    class Singleton
    {
    public:

        static T& GetInstance()
        {
            if (instance == nullptr)
            {
                instance = new T();
            }

            return *instance;
        }

        static void Release()
        {
            if (instance)
            {
                delete instance;
                instance = nullptr;
            }
        }

    protected:

        Singleton() {};
        ~Singleton() { Release(); };

        static T* instance;
    };

    template <class T>
    T* Singleton<T>::instance = nullptr;



    template <class T>
    class ThreadSafeSingleton
    {
    public:
        // Provide global access to the only instance of this class
        static T& GetInstance()
        {
            if (!m_pInstance) // This if statement prevents the costly Lock-step being required each time the instance is requested
            {
                std::lock_guard<std::mutex> lock(m_mConstructed); // Lock is required here though, to prevent multiple threads initialising multiple instances of the class when it turns out it has not been initialised yet
                if (!m_pInstance)                                  // Check to see if a previous thread has already initialised an instance in the time it took to acquire a lock.
                {
                    m_pInstance = new T();
                }
            }
            return *m_pInstance;
        }

        // Provide global access to release/delete this class
        static void Release()
        {
            // Technically this could have another enclosing if statement, but speed is much less of a problem as this should only be called once in the entire program.
            std::lock_guard<std::mutex> lock(m_mConstructed);
            if (m_pInstance)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }

    protected:
        // Only allow the class to be created and destroyed by itself
        ThreadSafeSingleton() { }
        ~ThreadSafeSingleton() { }

        static T* m_pInstance;
        // Keep a static instance pointer to refer to as required by the rest of the program
        static std::mutex m_mConstructed;

    private:
        ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
        ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
    };

    // Finally make sure that the instance is initialised to NULL at the start of the program
    template <class T>
    std::mutex ThreadSafeSingleton<T>::m_mConstructed;
    template <class T>
    T* ThreadSafeSingleton<T>::m_pInstance = nullptr;



}

#endif // !SINGLETON_H
