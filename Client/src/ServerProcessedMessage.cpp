#include "ServerProcessedMessage.hpp"
#include "Config.hpp"

ServerProcessedMessage::ServerProcessedMessage()
{

}

ServerProcessedMessage::~ServerProcessedMessage()
{

}

bool ServerProcessedMessage::isMessageProcessed(uint16_t ID)
{
    
    if(Config::getConfig()->getProtocol() == Protocol::TCP)
        return false;

    for (auto& CurrentID : ProcessedID)
    {
        if (CurrentID == ID)
        {
            return true;
        }
    }

    return false;
}

void ServerProcessedMessage::addProcessedMessage(uint16_t ID)
{
    
    if(Config::getConfig()->getProtocol() == Protocol::TCP)
        return;

    ProcessedID.push_back(ID);
}