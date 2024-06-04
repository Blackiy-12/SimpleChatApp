#include "UserData.hpp"


UserData::UserData()
{

}

UserData::~UserData()
{

}

uint16_t UserData::getMessageID()
{
    LastID++;
    return LastID;
}

bool UserData::isMessageProcessed(uint16_t MessageID)
{
    for (auto &ID : ProcessedMessageFromUser)
    {
        if (MessageID == ID)
            return true;
    }

    return false;
}

void UserData::addMessageFromUserToProcessed(uint16_t MessageID)
{
    ProcessedMessageFromUser.push_back(MessageID);
}
