#include "UserProcessedMessage.hpp"

#include "Config.hpp"


UserProcessedMessage::UserProcessedMessage()
{
}
    
UserProcessedMessage::~UserProcessedMessage()
{
}

uint16_t UserProcessedMessage::getNextMessageID()
{
    auto Result = NextMessageID;
    NextMessageID++;
    return Result;
}

bool UserProcessedMessage::wasWaitingReply(uint16_t ID)
{
    if(Config::getConfig()->getProtocol() == Protocol::TCP)
        return true;

    for (auto& curID : WantReply)
    {
        if (curID == ID)
        {
            return true;
        }
    }
    return false;
}

bool UserProcessedMessage::wasSended(uint16_t ID)
{
    if(Config::getConfig()->getProtocol() == Protocol::TCP)
        return true;

    for (auto& curID : SendedMessages)
    {
        if (curID == ID)
        {
            return true;
        }
    }
    return false;
}


void UserProcessedMessage::pushSendedMessage(uint16_t ID)
{
    if(Config::getConfig()->getProtocol() == Protocol::TCP)
        return;

    SendedMessages.push_back(ID);
}

void UserProcessedMessage::pushMessageThatWantReply(uint16_t ID)
{
    if(Config::getConfig()->getProtocol() == Protocol::TCP)
        return;

    WantReply.push_back(ID);
    wasSended(ID);
}