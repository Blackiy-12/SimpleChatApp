#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <arpa/inet.h>

#include "Protocol.hpp"
#include "Message.hpp"
#include "MessageBuffer.hpp"

class IConnection
{
public:
    virtual ~IConnection() {};

    virtual void createSocket() = 0;

    virtual void acceptConnection(int WelcomeSocket) = 0;

    virtual int getSocket() = 0; 

    virtual void bindSocket() = 0;

    virtual bool sendMessage(std::shared_ptr<IMessage> Message) = 0;

    virtual void receiveMessage() = 0;

    virtual struct addrinfo* getLastAddress() = 0;

    virtual void linkMessageBuffer(IMessageBuffer* Buffer) = 0;

    virtual std::string getIP() = 0;

    virtual std::string getPort() = 0;
};

class ConnectionCreator
{
public:
    static std::unique_ptr<IConnection> createConnection(Protocol Protocol, std::string ServerAddress, uint16_t ServerPort);

    static std::unique_ptr<IConnection> createConnection(Protocol Protocol, struct addrinfo* ServerInfo);
};