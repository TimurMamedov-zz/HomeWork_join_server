cmake_minimum_required(VERSION 3.9)

set(BOOST_ROOT          "C:/boost/boost_1_62_0/boost_mingw53")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)

set(HEADERS test_query_executor.h)
set(SOURCES test_query_executor.cpp)

add_executable(test_query_executor ${HEADERS} ${SOURCES})

set_target_properties (test_query_executor PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

set_target_properties (test_query_executor PROPERTIES
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(test_query_executor
        ${Boost_LIBRARIES}
)

target_include_directories(
    test_query_executor
    PUBLIC .
    )

enable_testing()

add_test (test_query_executor test_query_executor)
