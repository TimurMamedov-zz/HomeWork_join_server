#pragma once
#include <thread>
#include "thread_pool.h"
#include "join_session.h"
#include "parser.h"

class join_server
{
public:
    join_server(boost::asio::io_service& io_service,
                const ba::ip::tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint),
          socket_(io_service)/*, queue(data_cond, finish),
          parser(queue)*/
    {
        do_accept();
    }

    ~join_server()
    {
//        finish = true;
//        writeThread.join();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::make_shared<join_session>(std::move(socket_),
                                               join_sessions,
                                               pool.getQueue(join_sessions.size() % pool.getThreadsSize()))->start();
            }

            do_accept();
        });
    }

    std::condition_variable data_cond;
    std::set<join_session_ptr> join_sessions;
    ba::ip::tcp::acceptor acceptor_;
    ba::ip::tcp::socket socket_;
    Parser parser;
    thread_pool pool;
};
