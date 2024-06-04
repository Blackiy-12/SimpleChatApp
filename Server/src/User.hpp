#pragma once 

#include <string>
#include <memory>
#include <arpa/inet.h>

#include "Connection.hpp"
#include "Protocol.hpp"
#include "UserData.hpp"
#include "ChannelHolder.hpp"
#include "MessageBuffer.hpp"

class AuthenticationHandler;

enum class UserState
{
    AUTH,
    OPEN,
    ERR,
    BYE
};

class User
{
public:

    User(Protocol Protocol, ChannelHolder* Holder, AuthenticationHandler* Handler);

    ~User();

    void acceptConnection(int WelcomeSocket, struct addrinfo* Address);

    void createConnection(struct addrinfo* Address);

    int getUserSocket();

    void sendMessageToUser(std::string Nickname, std::string Content);

    void receiveMessage();

    void processAuth(IAuthMessage* Message);

    void sendBye();

    void endWithError(std::string ErrorContent);

    std::string getLastNick();

    UserState getCurrentState();

private:
    void sendReply(bool Result, std::string ReplyContent, uint16_t RefID);

    void joinChannel(IJoinMessage* Message);

    void processMessage(ITextMessage* Message);

private:
    AuthenticationHandler* AuthHandler;

    ChannelHolder* Channels;

    Protocol UserProtocol;

    Channel* CurrentChannel;

    std::string Login;

    std::string LastNick;
    
    std::unique_ptr<IConnection> Connection;

    std::unique_ptr<UserData> Data = nullptr; //Only for udp  variant

    UserState State;

    std::unique_ptr<IMessageBuffer> MessageBuffer;
};