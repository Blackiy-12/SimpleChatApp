#pragma once

#include <string>
#include <vector>

class User;

class Channel
{
public:
    Channel(std::string Name);

    ~Channel();

    void sendMessageAllUsers(std::string Nick, std::string MessageContent, User* From);

    void join(std::string Nick, User* NewUser);

    void leave(std::string Nick, User* UserToDisconnect);

    bool isEmpty();

private: 
    std::string ChannelName;

    std::vector<User*> Users;
};