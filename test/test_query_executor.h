/*!
\file
\brief
*/
#pragma once
#define BOOST_TEST_MODULE join_query_executor

#include <boost/test/unit_test.hpp>
#include "../query_executor.h"
#include "../query_executor.cpp"

struct Fixture
{
    Fixture()
    {
        queueOK.emplace("OK\n");
        queueDuplicate.emplace("ERR duplicate 1\n");
    }

    void Intersection()
    {
        queueIntersection.emplace("3,violation,proposal\n");
        queueIntersection.emplace("4,quality,example\n");
        queueIntersection.emplace("5,precision,lake\n");
        queueIntersection.emplace("OK\n");
    }

    void Simmetric_difference()
    {
        queueSimmetric_difference.emplace("0,lean,\n");
        queueSimmetric_difference.emplace("1,sweater,\n");
        queueSimmetric_difference.emplace("2,frank,\n");
        queueSimmetric_difference.emplace("6,,flour\n");
        queueSimmetric_difference.emplace("7,,wonder\n");
        queueSimmetric_difference.emplace("8,,selection\n");
        queueSimmetric_difference.emplace("OK\n");
    }

    std::vector<std::string> getVectorFromQueue(std::queue<std::string> queue)
    {
        std::vector<std::string> vector;
        vector.reserve(queue.size());
        while(!queue.empty())
        {
            auto str = queue.front();
            queue.pop();
            vector.emplace_back(std::move(str));
        }
        return vector;
    }

    std::queue<std::string> queueOK;
    std::queue<std::string> queueDuplicate;
    std::queue<std::string> queueIntersection;
    std::queue<std::string> queueSimmetric_difference;

    QueryExecutor executor;
};
