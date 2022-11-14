#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

#define THREAD_POOL_SIZE 4

class Event;

class EventThreadPool
{
public:
    void Start();
    void QueueJob(const std::function<void(Event*)>& job);
    void Stop();
    void StopWithJoin();
    bool Busy();

private:
    void threadLoop();

    bool m_Should_Terminate = false;            // Tells threads to stop looking for jobs
    std::mutex m_Queue_Mutex;                   // Prevents data races to the job queue
    std::condition_variable m_Mutex_Condition;  // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> m_Threads;
    std::queue<std::function<void(Event*)>> m_Jobs;
};