#pragma once
#include <thread>
#include "join_session.h"
#include "parser.h"

class join_server
{
public:
    join_server(boost::asio::io_service& io_service,
                const ba::ip::tcp::endpoint& endpoint)
        : finish(false), acceptor_(io_service, endpoint),
          socket_(io_service), queue(data_cond, finish),
          parser(queue), writeThread(std::ref(parser))
    {
        do_accept();
    }

    ~join_server()
    {
        finish = true;
        writeThread.join();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::make_shared<join_session>(std::move(socket_), join_sessions, queue)->start();
            }

            do_accept();
        });
    }

    bool finish;
    ThreadSave_Queue<std::pair<std::string, join_session_ptr> > queue;
    std::condition_variable data_cond;
    std::set<join_session_ptr> join_sessions;
    ba::ip::tcp::acceptor acceptor_;
    ba::ip::tcp::socket socket_;
    Parser parser;
    std::thread writeThread;
};
