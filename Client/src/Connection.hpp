#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include "Protocol.hpp"
#include "Message.hpp"

class IConnection
{
public:
    virtual ~IConnection() {};

    virtual int getSocket() = 0;

    virtual void connectToServer() = 0;

    virtual void stopConnection() = 0;

    virtual bool sendMessage(std::shared_ptr<IMessage> Message) = 0;

    virtual std::shared_ptr<IMessage> receiveMessage() = 0;

    virtual bool isConnected() const = 0;
};

class ConnectionCreator
{
public:
    static std::shared_ptr<IConnection> createConnection();

};