#pragma once
#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include "threadsafe_queue.h"

namespace ba = boost::asio;

class join_session;

using join_session_ptr = std::shared_ptr<join_session>;

class join_session
        : public std::enable_shared_from_this<join_session>
{
public:
    join_session(ba::ip::tcp::socket socket,
                 std::set<join_session_ptr>& join_sessions,
                 ThreadSave_Queue<std::pair<std::string, join_session_ptr> >& queue_)
        : socket_(std::move(socket)),
          join_sessions_(join_sessions),
          queue(queue_)
    {
    }

    void start()
    {
        join_sessions_.insert(shared_from_this());
        do_read();
    }

    void do_write(std::string response)
    {
      auto self(shared_from_this());
      boost::asio::async_write(socket_,
          boost::asio::buffer(response, response.size()),
          [this, self, response](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (ec)
            {
               join_sessions_.erase(shared_from_this());
            }
          });
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_,
                                streambuf, '\n',
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::istream is(&streambuf);
                std::string line;
                std::getline(is, line);
                queue.push(std::pair<std::string, join_session_ptr>(line, self));
                do_read();
            }
            else
            {
                join_sessions_.erase(shared_from_this());
            }
        });
    }

    std::mutex readWriteMutex;
    boost::asio::streambuf streambuf;
    ba::ip::tcp::socket socket_;
    std::set<join_session_ptr>& join_sessions_;
    ThreadSave_Queue<std::pair<std::string, join_session_ptr> >& queue;
};
