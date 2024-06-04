#pragma once

#include "Protocol.hpp"
#include "Arguments.hpp"

class Config
{
public:
    static Config* getConfig();

    static void createConfig(Arguments Args);

    Protocol getProtocol() const {return CurrentProtocol;}
    
    std::string getServer() const { return Server; }
    
    uint16_t getServerPort() const { return Port ;}
    
    uint16_t getUDPTimeout() const { return Timeout; }

    uint8_t getNumberOfRetransmition() const { return RetransmitionNumber; }

private:
    Config();
    
    ~Config();

private:
    static Config* ConfigPtr;
    
    Protocol CurrentProtocol;

    std::string Server;
    
    uint16_t Port;
    
    uint16_t Timeout;
    
    uint8_t RetransmitionNumber;
};