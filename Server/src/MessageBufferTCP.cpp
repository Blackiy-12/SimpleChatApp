#include "MessageBufferTCP.hpp"


MessageBufferTCP::MessageBufferTCP()
{
    Buffer = {};
}

MessageBufferTCP::~MessageBufferTCP()
{

}

void MessageBufferTCP::addMessage(std::vector<uint8_t> Bytes)
{
    Buffer.insert(Buffer.end(), Bytes.begin(), Bytes.end());
}

void MessageBufferTCP::addMessage(std::shared_ptr<IMessage> Message)
{

}

bool MessageBufferTCP::hasMessage()
{
    bool PrevWasR = false;

    for (int i = 0; i < Buffer.size(); i++)
    {
        if (Buffer.at(i) == '\r')
        {
            PrevWasR = true;
            continue;
        }

        if (Buffer.at(i) == '\n' && PrevWasR)
        {
            return true;
        }

        PrevWasR = false;
    }

    return false;
}

std::shared_ptr<IMessage> MessageBufferTCP::getMessage()
{
    bool PrevWasR = false;
    int EndOfMessage = -1;

    for (int i = 0; i < Buffer.size(); i++)
    {
        if (Buffer.at(i) == '\r')
        {
            PrevWasR = true;
            continue;
        }

        if (Buffer.at(i) == '\n' && PrevWasR)
        {
            EndOfMessage = i;
            break;
        }

        PrevWasR = false;
    }

    if (EndOfMessage == -1)
    {
        return std::make_shared<NoneMessage>();
    }

    EndOfMessage++;

    std::vector<uint8_t> MessageBytes = {};

    MessageBytes.insert(MessageBytes.end(), Buffer.begin(), Buffer.begin() + EndOfMessage);

    Buffer.erase(Buffer.begin(), Buffer.begin() + EndOfMessage);

    return MessageCreator::translateMessage(Protocol::TCP, MessageBytes);
}