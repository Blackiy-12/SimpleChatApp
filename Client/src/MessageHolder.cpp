#include "MessageHolder.hpp"


MessageHolder::MessageHolder()
{

}

MessageHolder::~MessageHolder()
{

}

void MessageHolder::push(std::shared_ptr<IMessage> Message)
{
    Queue.push(Message);
}

std::shared_ptr<IMessage> MessageHolder::pop()
{
    auto MSG = Queue.front();
    Queue.pop();
    return MSG;
}

bool MessageHolder::isEmpty()
{
    return Queue.empty();
}