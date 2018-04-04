/*!
\file
\brief
*/
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include <functional>
#include "parser.h"
#include "data_base.h"

namespace ba = boost::asio;

class QueryExecutor
{
public:
    QueryExecutor();
    std::string execute(std::string query);

private:
    Parser parser;
    std::mutex mutexDB;
    std::unordered_map<std::string,
        std::function<std::string(const std::vector<std::string>&)> > func_hash;

    std::string insert(const std::vector<std::string>& words);
    std::string truncate(const std::vector<std::string>& words);
};

