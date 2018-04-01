/*!
\file
\brief
*/
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include "threadsafe_queue.h"

namespace ba = boost::asio;

class QueryExecutor
{
public:
    QueryExecutor(ba::ip::tcp::socket& socket_,
                  std::mutex& readWriteMutex_);

    std::string operator()(std::string query);
    virtual ~QueryExecutor(){}

private:
    ba::ip::tcp::socket& socket;
    std::mutex& readWriteMutex;
};

