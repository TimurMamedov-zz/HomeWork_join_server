#pragma once
#include <unordered_map>
#include <map>
#include <string>
#include <mutex>

using table = std::pair<std::mutex, std::unordered_map<int, std::string> >;

class DataBase
{
public:
    static std::map<std::string, table> tables;
};

//std::map<std::string, table> DataBase::tables;
