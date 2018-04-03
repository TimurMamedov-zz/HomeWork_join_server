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
    explicit ThreadSave_Queue(std::condition_variable& data_cond_,
                              const std::atomic_bool& done_)
        : data_cond(data_cond_), done(done_){}
    ThreadSave_Queue(ThreadSave_Queue&& queue_)
        :queue(std::move(queue_.queue)),
          data_cond(queue_.data_cond), done(queue_.done){}
    ThreadSave_Queue(const ThreadSave_Queue& ) = delete;
    ThreadSave_Queue& operator =(const ThreadSave_Queue& ) = delete;
    ~ThreadSave_Queue() = default;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mt);
        queue.emplace(std::move(new_value));
        data_cond.notify_one();
    }

    bool wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mt);
        if(done && queue.empty())
            return false;
        data_cond.wait(lk, [this]{ return (!queue.empty() || done);});
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
    std::condition_variable& data_cond;
    const std::atomic_bool& done;
};
