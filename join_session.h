#pragma once
#include <memory>
#include <set>
#include "query_executor.h"

class join_session;

using join_session_ptr = std::shared_ptr<join_session>;

class join_session
        : public std::enable_shared_from_this<join_session>
{
public:
    join_session(ba::ip::tcp::socket socket,
                 std::set<join_session_ptr>& join_sessions)
        : socket_(std::move(socket)),
          join_sessions_(join_sessions)
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

                do_write(queryExecutor(line));
                do_read();
            }
            else
            {
                join_sessions_.erase(shared_from_this());
            }
        });
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

    std::mutex socketMutex;
    QueryExecutor queryExecutor;
    boost::asio::streambuf streambuf;
    ba::ip::tcp::socket socket_;
    std::set<join_session_ptr>& join_sessions_;
};
