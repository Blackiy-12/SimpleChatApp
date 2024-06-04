#include "Connection.hpp"

#include "ConnectionUDP.hpp"
#include "ConnectionTCP.hpp"


std::unique_ptr<IConnection> ConnectionCreator::createConnection(Protocol Protocol, std::string ServerAddress, uint16_t ServerPort)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_unique<ConnectionUDP>(ServerAddress, ServerPort);
    }
    else
    {
        return std::make_unique<ConnectionTCP>(ServerAddress, ServerPort);
    }
}

std::unique_ptr<IConnection> ConnectionCreator::createConnection(Protocol Protocol, struct addrinfo* ServerInfo)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_unique<ConnectionUDP>(ServerInfo);
    }
    else
    {
        return std::make_unique<ConnectionTCP>(ServerInfo);
    }
}