#include "Config.hpp"

Config* Config::ConfigPtr = nullptr;

Config* Config::getConfig()
{
    if (ConfigPtr == nullptr)
        ConfigPtr = new Config();

    return ConfigPtr;
}

void Config::createConfig(Arguments Args)
{
    if (ConfigPtr == nullptr)
        getConfig();

    ConfigPtr->Server = Args.getServer();
    ConfigPtr->Port = Args.getServerPort();
    ConfigPtr->RetransmitionNumber = Args.getNumberOfRetransmition();
    ConfigPtr->Timeout = Args.getUDPTimeout();
}

Config::Config()
{

}
    
Config::~Config()
{

}