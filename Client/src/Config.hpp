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

    bool isAppInError() { return AppInError; }

    void setAppInError() { AppInError = true; }

    void setNickname(std::string Nickname) { Name = Nickname; }

    std::string getNickname() { return Name; }

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

    bool AppInError = false;

    std::string Name = "";
};