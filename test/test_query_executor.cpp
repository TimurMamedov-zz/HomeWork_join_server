#include "test_query_executor.h"
#include <iostream>

BOOST_AUTO_TEST_SUITE(Test_QueryExecutor)

BOOST_FIXTURE_TEST_CASE(test_insert, Fixture)
{
    auto query = executor.execute("INSERT A 0 lean\n");
    auto currentVector = getVectorFromQueue(std::move(query));
    auto expectedVector = getVectorFromQueue(std::move(queueOK));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());

    executor.execute("INSERT A 1 lean\n");
    query = executor.execute("INSERT A 1 understand\n");
    currentVector = getVectorFromQueue(std::move(query));
    expectedVector = getVectorFromQueue(std::move(queueDuplicate));
    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());
}

BOOST_FIXTURE_TEST_CASE(test_truncate, Fixture)
{
    executor.execute("INSERT A 0 lean\n");
    auto queryA = executor.execute("TRUNCATE A\n");
    auto currentVector = getVectorFromQueue(std::move(queryA));
    auto expectedVector = getVectorFromQueue(std::move(queueOK));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());

    auto queryB = executor.execute("TRUNCATE B\n");
    currentVector = getVectorFromQueue(std::move(queryB));
    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());
}

BOOST_FIXTURE_TEST_CASE(test_intersection, Fixture)
{
    auto expectedVector = getVectorFromQueue(std::move(queueOK));
    executor.execute("TRUNCATE A\n");
    executor.execute("INSERT A 0 lean\n");
    executor.execute("INSERT A 1 sweater\n");
    executor.execute("INSERT A 2 frank\n");
    executor.execute("INSERT A 3 violation\n");
    executor.execute("INSERT A 4 quality\n");
    auto query = executor.execute("INSERT A 5 precision\n");
    auto currentVector = getVectorFromQueue(std::move(query));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());

    executor.execute("TRUNCATE B\n");
    executor.execute("INSERT B 3 proposal\n");
    executor.execute("INSERT B 4 example\n");
    executor.execute("INSERT B 5 lake\n");
    executor.execute("INSERT B 6 flour\n");
    executor.execute("INSERT B 7 wonder\n");
    query = executor.execute("INSERT B 8 selection\n");
    currentVector = getVectorFromQueue(std::move(query));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());

    Intersection();
    query = executor.execute("INTERSECTION\n");
    std::cout << query.size() << std::endl;
    currentVector = getVectorFromQueue(std::move(query));
    expectedVector = getVectorFromQueue(std::move(queueIntersection));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());
}

BOOST_FIXTURE_TEST_CASE(test_simmetric_difference, Fixture)
{
    auto expectedVector = getVectorFromQueue(std::move(queueOK));
    executor.execute("TRUNCATE A\n");
    executor.execute("INSERT A 0 lean\n");
    executor.execute("INSERT A 1 sweater\n");
    executor.execute("INSERT A 2 frank\n");
    executor.execute("INSERT A 3 violation\n");
    executor.execute("INSERT A 4 quality\n");
    auto query = executor.execute("INSERT A 5 precision\n");
    auto currentVector = getVectorFromQueue(std::move(query));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());

    executor.execute("TRUNCATE B\n");
    executor.execute("INSERT B 3 proposal\n");
    executor.execute("INSERT B 4 example\n");
    executor.execute("INSERT B 5 lake\n");
    executor.execute("INSERT B 6 flour\n");
    executor.execute("INSERT B 7 wonder\n");
    query = executor.execute("INSERT B 8 selection\n");
    currentVector = getVectorFromQueue(std::move(query));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());

    Simmetric_difference();
    query = executor.execute("SYMMETRIC_DIFFERENCE\n");
    currentVector = getVectorFromQueue(std::move(query));
    expectedVector = getVectorFromQueue(std::move(queueSimmetric_difference));

    BOOST_CHECK_EQUAL_COLLECTIONS(currentVector.begin(), currentVector.end(),
                                  expectedVector.begin(), expectedVector.end());
}

BOOST_AUTO_TEST_SUITE_END()
