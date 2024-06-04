#pragma once

#include <memory>
#include <queue>
#include "Message.hpp"

class MessageHolder
{
public:
    MessageHolder();

    ~MessageHolder();

    void push(std::shared_ptr<IMessage> Message);

    std::shared_ptr<IMessage> pop();

    bool isEmpty();

private:
    std::queue<std::shared_ptr<IMessage>> Queue;
};