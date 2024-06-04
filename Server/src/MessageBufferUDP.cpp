#include "MessageBufferUDP.hpp"


MessageBufferUDP::MessageBufferUDP()
{
    Buffer = {};
}

MessageBufferUDP::~MessageBufferUDP()
{

}

void MessageBufferUDP::addMessage(std::vector<uint8_t> Bytes)
{

}

void MessageBufferUDP::addMessage(std::shared_ptr<IMessage> Message)
{
    Buffer.push(Message);
}

bool MessageBufferUDP::hasMessage()
{
    return ! Buffer.empty();
}

std::shared_ptr<IMessage> MessageBufferUDP::getMessage()
{
    if (Buffer.empty())
    {
        return std::make_shared<NoneMessage>();
    }
    auto TMP = Buffer.front();

    Buffer.pop();

    return TMP;
}