#include "Channel.hpp"

#include "User.hpp"

Channel::Channel(std::string Name)
{
    ChannelName = Name;
    Users = {};
}

Channel::~Channel()
{

}

void Channel::sendMessageAllUsers(std::string Nick, std::string MessageContent, User* From)
{
    for(auto &User : Users)
    {
        if (From == User)
        {
            continue;
        }

        User->sendMessageToUser(Nick, MessageContent);
    }
}

void Channel::join(std::string Nick, User* NewUser)
{
    std::string Message = Nick;

    Message.append(" has joined ");
    Message.append(ChannelName);
    Message.append(".");

    for(auto &User : Users)
    {
        User->sendMessageToUser("Server", Message);
    }

    Users.push_back(NewUser);
}

void Channel::leave(std::string Nick, User* UserToDisconnect)
{
    
    std::string Message = Nick;

    Message.append(" has left ");
    Message.append(ChannelName);
    Message.append(".");

    for(unsigned int i = 0; i < Users.size(); i++)
    {
        if (UserToDisconnect == Users.at(i))
        {
            Users.erase(Users.begin() + i);
            i--;
            continue;
        }

        Users.at(i)->sendMessageToUser("Server", Message);
    }
}

bool Channel::isEmpty()
{
    if (Users.size() == 0)
    {
        return true;
    }

    return false;
}