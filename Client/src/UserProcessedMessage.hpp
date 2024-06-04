#pragma once

#include <vector>
#include <cstdint>

class UserProcessedMessage 
{
public:
    UserProcessedMessage();

    ~UserProcessedMessage();

    void pushSendedMessage(uint16_t ID);

    void pushMessageThatWantReply(uint16_t ID);

    uint16_t getNextMessageID();

    bool wasWaitingReply(uint16_t ID);

    bool wasSended(uint16_t ID);

private:
    uint16_t NextMessageID = 0x00;

    std::vector<uint16_t> SendedMessages;

    std::vector<uint16_t> WantReply;
};