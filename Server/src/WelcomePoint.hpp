#pragma once 

#include "User.hpp"
#include "Connection.hpp"
#include "MessageBuffer.hpp"

#include <memory>

class ChannelHolder;

class AuthenticationHandler;

class WelcomePoint
{
public:

    WelcomePoint();
   
    ~WelcomePoint();

    void createConnection(Protocol Protocol, std::string ServerAddress, uint16_t ServerPort);

    void linkToAuthenticationHandler(AuthenticationHandler* Handler);

    std::shared_ptr<User> processNewUser(ChannelHolder* Channels);

    int getSocket();

private:
    std::unique_ptr<IConnection> Connection;

    AuthenticationHandler* AuthenticationHandlerPtr;

    Protocol PointProtocol;

    std::unique_ptr<IMessageBuffer> MessageBuffer;
};