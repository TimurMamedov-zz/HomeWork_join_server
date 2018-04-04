#include "query_executor.h"
#include <sstream>
#include <algorithm>
#include <cctype>

std::map<std::string, table> DataBase::tables;

QueryExecutor::QueryExecutor()
{
    DataBase::tables.emplace("A", table());
    DataBase::tables.emplace("B", table());

    auto InsertFunc = [this](const std::vector<std::string>& words)
    {
        return this->insert(words);
    };

    auto TruncateFunc = [this](const std::vector<std::string>& words)
    {
        return this->truncate(words);
    };

    auto IntersectionFunc = [this](const std::vector<std::string>& words)
    {
        return this->intersection(words);
    };

    auto SimmetricDifferenceFunc = [this](const std::vector<std::string>& words)
    {
        return this->simmetric_difference(words);
    };

    func_hash.emplace("INSERT", InsertFunc);
    func_hash.emplace("TRUNCATE", TruncateFunc);
    func_hash.emplace("INTERSECTION", IntersectionFunc);
    func_hash.emplace("SYMMETRIC_DIFFERENCE", SimmetricDifferenceFunc);
}

std::string QueryExecutor::execute(std::string query)
{
    std::string response;
    std::istringstream iss(query);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));
    if(words.size() > 4 ||
            words.size() == 3 ||
            words.empty())
        response = "ERR invalid query";
    else
    {
        for (auto & word: words)
            for (auto & c: word)
                c = toupper(c);

        response = func_hash[words[0]](words);
    }
    return response;
}

std::string QueryExecutor::insert(const std::vector<std::string> &words)
{
    std::string response;
    auto id = std::stoi(words[2]);
    if(DataBase::tables.find(words[1]) != DataBase::tables.end())
    {
        std::lock_guard<std::mutex> lk(mutexDB);
        auto& table_hash = DataBase::tables[words[1]];
        if(table_hash.second.find(id) == table_hash.second.end())
        {
            table_hash.second.emplace(id, words[3]);
            response = "OK";
        }
        else
            response = "ERR duplicate " + std::to_string(id);
    }
    else
        response = "ERR invalid name table";
    return response;
}

std::string QueryExecutor::truncate(const std::vector<std::string> &words)
{
    std::string response;
    if(DataBase::tables.find(words[1]) != DataBase::tables.end())
    {
        std::lock_guard<std::mutex> lk(mutexDB);
        DataBase::tables[words[1]].second.clear();
        response = "OK";
    }
    else
        response = "ERR invalid name table";
    return response;
}

std::string QueryExecutor::intersection(const std::vector<std::string> &words)
{
    std::string response;
    return response;
}

std::string QueryExecutor::simmetric_difference(const std::vector<std::string> &words)
{
    std::string response;
    return response;
}
