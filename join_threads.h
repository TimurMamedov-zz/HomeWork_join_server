#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>
#include "threadsafe_queue.h"

class join_threads
{
public:
    join_threads(std::vector<std::thread>& threads_,
                 std::vector<std::condition_variable>& cond_var_vector_)
        :threads(threads_), cond_var_vector(cond_var_vector_) {}
    ~join_threads()
    {
        for(auto i = std::size_t{0}; i < threads.size(); i++)
        {
            if(threads[i].joinable())
            {
                cond_var_vector[i].notify_one();
                threads[i].join();
            }
        }
    }

private:
    std::vector<std::thread>& threads;
    std::vector<std::condition_variable>& cond_var_vector;
};
