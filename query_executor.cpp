#include "query_executor.h"
#include <sstream>
#include <iterator>
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
        return this->intersection();
    };

    auto SimmetricDifferenceFunc = [this](const std::vector<std::string>& words)
    {
        return this->simmetric_difference();
    };

    func_hash.emplace("INSERT", InsertFunc);
    func_hash.emplace("TRUNCATE", TruncateFunc);
    func_hash.emplace("INTERSECTION", IntersectionFunc);
    func_hash.emplace("SYMMETRIC_DIFFERENCE", SimmetricDifferenceFunc);
}

std::queue<std::string> QueryExecutor::execute(std::string query)
{
    std::queue<std::string> response;

    std::istringstream iss(query);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));
    if(words.size() > 4 ||
            words.size() == 3 ||
            words.empty())
        response.emplace("ERR invalid query\n");
    else
    {
        for (auto & c: words[0])
            c = toupper(c);
        if(func_hash.find(words[0]) != func_hash.end())
            response = func_hash[words[0]](words);
    }
    return response;
}

std::queue<std::string> QueryExecutor::insert(const std::vector<std::string> &words)
{
    std::queue<std::string> response;
    if(words.size() == 4)
    {
        auto id = std::stoi(words[2]);

        if(DataBase::tables.find(words[1]) != DataBase::tables.end())
        {
            std::lock_guard<std::mutex> lk(*DataBase::tables[words[1]].mutex);
            auto& table = DataBase::tables[words[1]];
            if(table.map.find(id) == table.map.end())
            {
                table.map.emplace(id, words[3]);
                response.emplace("OK\n");
            }
            else
                response.emplace("ERR duplicate " + std::to_string(id) + "\n");
        }
        else
            response.emplace("ERR invalid name table\n");
    }
    else
        response.emplace("ERR invalid query\n");
    return response;
}

std::queue<std::string> QueryExecutor::truncate(const std::vector<std::string> &words)
{
    std::queue<std::string> response;
    if(words.size() == 2)
    {
        if(DataBase::tables.find(words[1]) != DataBase::tables.end())
        {
            std::lock_guard<std::mutex> lk(*DataBase::tables[words[1]].mutex);
            DataBase::tables[words[1]].map.clear();
            response.emplace("OK\n");
        }
        else
            response.emplace("ERR invalid name table\n");
    }
    else
        response.emplace("ERR invalid query\n");
    return response;
}

std::queue<std::string> QueryExecutor::intersection()
{
    std::queue<std::string> response;
    applyFunction(&QueryExecutor::set_intersection, response);
    return response;
}

std::queue<std::string> QueryExecutor::simmetric_difference()
{
    std::queue<std::string> response;
    applyFunction(&QueryExecutor::set_symmetric_difference, response);
    return response;
}

void QueryExecutor::set_intersection(InputIterator first1, InputIterator last1,
                                     InputIterator first2, InputIterator last2,
                                     Container &container)
{
    while (first1 != last1 && first2 != last2)
        if (*first1 < *first2)
            ++first1;
        else if (*first2 < *first1)
            ++first2;
        else
        {
            container.emplace(std::to_string((*first1).first) + "," +
                              (*first1).second + "," +
                              (*first2).second + "\n");
            ++first1;
            ++first2;
        }
}

void QueryExecutor::set_symmetric_difference(InputIterator first1, InputIterator last1,
                                             InputIterator first2, InputIterator last2,
                                             Container &container)
{
    using value_type = decltype(*first1);
    while (first1 != last1)
    {
        if (first2 == last2)
        {
            std::for_each(first1, last1, [&container](value_type& value)
            {
                container.emplace(std::to_string(value.first) + "," +
                                  value.second + "," + "\n");
            });
            return;
        }
        if (*first1 < *first2)
        {
            container.emplace(std::to_string((*first1).first) + "," +
                              (*first1).second + "," + "\n");
            ++first1;
        }
        else
        {
            if (*first2 < *first1)
            {
                container.emplace(std::to_string((*first2).first) + "," + "," +
                                  (*first2).second + "\n");
            }
            else
            {
                ++first1;
            }
            ++first2;
        }
    }
    std::for_each(first2, last2, [&container](value_type& value)
    {
        container.emplace(std::to_string(value.first) + "," + "," +
                          value.second + "\n");
    });
}

template<typename Func, typename Container>
void QueryExecutor::applyFunction(Func func, Container &container)
{
    std::map<int, std::string> map_A, map_B;
    {
        std::lock(*DataBase::tables["A"].mutex, *DataBase::tables["B"].mutex);
        std::lock_guard<std::mutex> lk1(*DataBase::tables["A"].mutex, std::adopt_lock);
        std::lock_guard<std::mutex> lk2(*DataBase::tables["B"].mutex, std::adopt_lock);
        map_A = DataBase::tables["A"].map;
        map_B = DataBase::tables["B"].map;
    }
    (this->*func)(map_A.begin(), map_A.end(),
         map_B.begin(), map_B.end(),
         container);
    container.emplace("OK\n");
}
