#include "MessageBuffer.hpp"

#include "MessageBufferTCP.hpp"
#include "MessageBufferUDP.hpp"

std::unique_ptr<IMessageBuffer> MessageBufferCreator::createMessageBuffer(Protocol Protocol)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_unique<MessageBufferUDP>();
    }
    else
    {
        return std::make_unique<MessageBufferTCP>();
    }
}