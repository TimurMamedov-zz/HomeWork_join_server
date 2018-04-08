#pragma once
#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include "threadsafe_queue.h"
#include "query_executor.h"

#include <iostream>

namespace ba = boost::asio;

class join_session;

using join_session_ptr = std::shared_ptr<join_session>;

class join_session
        : public std::enable_shared_from_this<join_session>
{
public:
    join_session(ba::ip::tcp::socket socket,
                 std::set<join_session_ptr>& join_sessions,
                 ThreadSave_Queue<std::function<void()> >& queue_,
                 QueryExecutor& executor_)
        : socket_(std::move(socket)),
          join_sessions_(join_sessions),
          queue(queue_), executor(executor_)
    {
    }

    void start()
    {
        join_sessions_.insert(shared_from_this());
        do_read();
    }

private:
    void do_read()
    {
        std::lock_guard<std::mutex> lk(readWriteMutex);
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_,
                                      streambuf, '\n',
                                      [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::istream is(&streambuf);
                std::string query;
                std::getline(is, query);
                queue.push([this, self, query]()
                {
                    this->do_write(executor.execute(query));
                });
                do_read();
            }
            else
            {
                join_sessions_.erase(shared_from_this());
            }
        });
    }

    void do_write(std::queue<std::string> response)
    {
        std::lock_guard<std::mutex> lk(readWriteMutex);
        if(!response.empty())
        {
            auto self(shared_from_this());
            auto front = response.front();
            response.pop();
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(front, front.size()),
                                     [this, self, response](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    if(!response.empty())
                    {
                        this->do_write(std::move(response));
                    }
                }
                else
                    join_sessions_.erase(shared_from_this());
            });
        }
        else
            std::cerr << "response is empty!\n";
    }

    std::mutex readWriteMutex;
    boost::asio::streambuf streambuf;
    ba::ip::tcp::socket socket_;
    std::set<join_session_ptr>& join_sessions_;
    ThreadSave_Queue<std::function<void()> >& queue;
    QueryExecutor& executor;
};
