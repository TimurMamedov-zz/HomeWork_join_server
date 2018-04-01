#pragma once
#include "join_session.h"

class join_server
{
public:
    join_server(boost::asio::io_service& io_service,
                const ba::ip::tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint),
          socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::make_shared<join_session>(std::move(socket_), join_sessions)->start();
            }

            do_accept();
        });
    }
    std::set<join_session_ptr> join_sessions;
    ba::ip::tcp::acceptor acceptor_;
    ba::ip::tcp::socket socket_;
};
