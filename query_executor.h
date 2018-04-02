/*!
\file
\brief
*/
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <mutex>

namespace ba = boost::asio;

class QueryExecutor
{
public:
    QueryExecutor();
    bool execute();

//private:
//    std::mutex& readWriteMutex;
};

