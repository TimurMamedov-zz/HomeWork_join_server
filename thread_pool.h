#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include "threadsafe_queue.h"

class join_threads
{
public:
    join_threads(std::vector<std::thread>& threads_)
        :threads(threads_){}
    ~join_threads()
    {
        for(auto& thread : threads)
        {
            if(thread.joinable())
                thread.join();
        }
    }

private:
    std::vector<std::thread>& threads;
};

class thread_pool
{
public:
    thread_pool()
        :done(false), joiner(threads)
    {
        try
        {
            auto thread_count = std::thread::hardware_concurrency();
            if(!thread_count)
                thread_count = 1;
            std::cout << "thread_count: " << thread_count << std::endl;
            queue_vector.resize(thread_count);
            threads.resize(thread_count);
            for(auto i = std::size_t{0}; i < thread_count; i++)
            {
                queue_vector.emplace_back();
                threads.emplace_back(std::thread(&thread_pool::worker_thread, this, i));
            }
        }
        catch(...)
        {
            done = true;
            throw;
        }
    }

    std::size_t getThreadsSize()
    {
        return threads.size();
    }

    ThreadSave_Queue<std::function<void()> >& getQueue(std::size_t index)
    {
        return queue_vector[index];
    }

    ~thread_pool()
    {
        done = true;
    }

private:
    std::atomic_bool done;
    std::vector<ThreadSave_Queue<std::function<void()> > > queue_vector;
    std::vector<std::thread> threads;

    join_threads joiner;

    void worker_thread(std::size_t index)
    {
        while (!done)
        {
           std::function<void()> task;
           if(queue_vector[index].try_pop(task))
           {
               task();
           }
           else
           {
               std::this_thread::yield();
           }
        }
    }
};
