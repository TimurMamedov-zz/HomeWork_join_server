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
    Parser(ThreadSave_Queue<std::pair<std::string, join_session_ptr> >& queue_);
    void operator ()();

private:
    QueryExecutor executor;
    ThreadSave_Queue<std::pair<std::string, join_session_ptr> >& queue;
};
