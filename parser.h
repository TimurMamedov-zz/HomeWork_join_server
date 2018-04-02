/*!
\file
\brief
*/
#pragma once
#include "query_executor.h"
#include "threadsafe_queue.h"

class join_session;

using join_session_ptr = std::shared_ptr<join_session>;

class Parser
{
public:
    Parser();
    void operator ()();

private:
    QueryExecutor executor;
};
