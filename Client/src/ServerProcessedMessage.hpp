#pragma once 

#include <cstdint>
#include <vector>

class ServerProcessedMessage
{
public:
    ServerProcessedMessage();
    ~ServerProcessedMessage();

    bool isMessageProcessed(uint16_t ID);

    void addProcessedMessage(uint16_t ID);

private:
    std::vector<uint16_t> ProcessedID;
};