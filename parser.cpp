#include "parser.h"
#include "join_session.h"

Parser::Parser(ThreadSave_Queue<std::pair<std::string, join_session_ptr> > &queue_)
    :queue(queue_)
{

}

void Parser::operator ()()
{
    while(true)
    {
        std::pair<std::string, join_session_ptr> pair;
        if(queue.wait_and_pop(pair))
        {

        }
        else
            break;
    }
}
