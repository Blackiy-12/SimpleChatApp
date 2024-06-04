#pragma once

#include <cstdint>
#include <vector>

class UserData
{
public:
    UserData();

    ~UserData();

    uint16_t getMessageID();

    bool isMessageProcessed(uint16_t MessageID);

    void addMessageFromUserToProcessed(uint16_t MessageID);

private:
    std::vector<uint16_t> ProcessedMessageFromUser;

    uint16_t LastID = -1;
};