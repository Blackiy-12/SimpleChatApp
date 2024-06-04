#include "Connection.hpp"

#include "ConnectionUDP.hpp"
#include "ConnectionTCP.hpp"
#include "Config.hpp"

std::shared_ptr<IConnection> ConnectionCreator::createConnection()
{
    if (Config::getConfig()->getProtocol() == Protocol::UDP)
    {
        return std::make_shared<ConnectionUDP>(Config::getConfig()->getServer(), Config::getConfig()->getServerPort());
    }
    else 
    {
        return std::make_shared<ConnectionTCP>(Config::getConfig()->getServer(), Config::getConfig()->getServerPort());
    }
}