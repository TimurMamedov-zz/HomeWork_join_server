#pragma once
#include <map>
#include <string>
#include <mutex>
#include <memory>

struct table
{
    std::unique_ptr<std::mutex> mutex;
    std::map<int, std::string> map;

    table()
    {
        mutex = std::make_unique<std::mutex>();
    }
};

class DataBase
{
public:
    static std::map<std::string, table> tables;
};

//std::map<std::string, table> DataBase::tables;
