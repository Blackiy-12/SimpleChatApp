#pragma once

#include "Message.hpp"

#include <memory>

class IMessageBuffer
{
public:
    virtual ~IMessageBuffer() {};

    virtual void addMessage(std::vector<uint8_t> Bytes) = 0;

    virtual void addMessage(std::shared_ptr<IMessage> Message) = 0;

    virtual bool hasMessage() = 0;

    virtual std::shared_ptr<IMessage> getMessage() = 0;
};

class MessageBufferCreator
{
public:
    static std::unique_ptr<IMessageBuffer> createMessageBuffer(Protocol Protocol);
};