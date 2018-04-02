/*!
\file
\brief
*/
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

template<typename T>
class ThreadSave_Queue
{
public:
    explicit ThreadSave_Queue() = default;
    ThreadSave_Queue(ThreadSave_Queue&& queue_){}
    ThreadSave_Queue(const ThreadSave_Queue& ) = delete;
    ThreadSave_Queue& operator =(const ThreadSave_Queue& ) = delete;
    ~ThreadSave_Queue() = default;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mt);
        queue.emplace(std::move(new_value));
    }

    bool try_pop(T& value) noexcept
    {
        std::lock_guard<std::mutex> lk(mt);
        if(queue.empty())
            return false;
        value = queue.front();
        queue.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mt);
        return queue.empty();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mt;
};
