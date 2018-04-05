/*!
\file
\brief
*/
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include <functional>
#include <unordered_map>
#include <queue>
#include "parser.h"
#include "data_base.h"

namespace ba = boost::asio;

class QueryExecutor
{
public:
    QueryExecutor();
    std::queue<std::string>execute(std::string query);

private:
    Parser parser;
    std::unordered_map<std::string,
        std::function<std::queue<std::string>(const std::vector<std::string>&)> > func_hash;

    std::queue<std::string> insert(const std::vector<std::string>& words);
    std::queue<std::string> truncate(const std::vector<std::string>& words);

    template<typename InputIterator, typename Container>
    void set_intersection(InputIterator first1, InputIterator last1,
                     InputIterator first2, InputIterator last2,
                     Container& container);
    std::queue<std::string> intersection(const std::vector<std::string>& words);

    template<class InputIterator, typename Container>
    void set_symmetric_difference(InputIterator first1, InputIterator last1,
                                      InputIterator first2, InputIterator last2,
                                      Container& container);
    std::queue<std::string> simmetric_difference(const std::vector<std::string>& words);

    template<typename Func, typename Container>
    void applyFunction(Func func, Container& container);
};

