#include "EventThreadPool.h"
#include "Event/Event.h"

/* The anonymous code blocks are used so that when they are exited,
the std::unique_lock variables created within them go out of scope, unlocking the mutex. */

void EventThreadPool::Start()
{
    m_Threads.resize(THREAD_POOL_SIZE);
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        //std::thread newThread([this] {this->threadLoop(); });
        //m_Threads.emplace_back(newThread);
        m_Threads.at(i) = std::thread([this] {this->threadLoop(); });;
    }
}

void EventThreadPool::threadLoop()
{
    while (true)
    {
        std::function<void(Event*)> job;
        {
            std::unique_lock<std::mutex> lock(m_Queue_Mutex);
            m_Mutex_Condition.wait(lock, [this] {
                return (!m_Jobs.empty() || m_Should_Terminate);
            });

            if (m_Should_Terminate)
            {
                return;
            }

            job = m_Jobs.front();
            m_Jobs.pop();
        }
        job(nullptr);
    }
}

void EventThreadPool::QueueJob(const std::function<void(Event*)>& job)
{
    {
        std::unique_lock<std::mutex> lock(m_Queue_Mutex);
        m_Jobs.push(job);
    }
    m_Mutex_Condition.notify_one();
}

bool EventThreadPool::Busy()
{
    bool poolBusy;
    {
        std::unique_lock<std::mutex> lock(m_Queue_Mutex);
        poolBusy = m_Jobs.empty();
    }
    return poolBusy;
}

void EventThreadPool::Stop()
{
    {
        std::unique_lock<std::mutex> lock(m_Queue_Mutex);
        m_Should_Terminate = true;
    }
    m_Mutex_Condition.notify_all();

    m_Threads.clear();
}

void EventThreadPool::StopWithJoin()
{
    {
        std::unique_lock<std::mutex> lock(m_Queue_Mutex);
        m_Should_Terminate = true;
    }
    m_Mutex_Condition.notify_all();

    for (std::thread& active_thread : m_Threads)
    {
        active_thread.join();
    }

    m_Threads.clear();
}