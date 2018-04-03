#include <iostream>
#include "join_server.h"

int main(int argc, char *argv[])
{
    try
    {
//        if (argc != 2)
//        {
//            std::cerr << "Usage:  join_session <port>\n";
//            return 1;
//        }

        ba::io_service io_service;

//        ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), std::atoi(argv[1]));
        ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), 9000);
        join_server server(io_service, endpoint);

        io_service.run();

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
