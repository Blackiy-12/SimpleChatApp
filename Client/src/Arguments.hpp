#pragma once
#include <string>
#include <cstdint>
#include "Protocol.hpp"

class Arguments
{
public:
    Arguments() {};
    ~Arguments() {};

    bool isCompleted() const;

    void printHelpInfo();

    //Setters
    void setProtocol(Protocol NewProtocol);
    void setServer(std::string NewServer);
    void setServerPort(uint16_t NewPort);
    void setUDPTimeout(uint16_t Time);
    void setNumberOfRetransmition(uint8_t NewNumber);
    void setHelpArgument();

    //Getters
    Protocol getProtocol() const;
    std::string getServer() const;
    uint16_t getServerPort() const;
    uint16_t getUDPTimeout() const;
    uint8_t getNumberOfRetransmition() const;
    bool needToPrintHelp();
    
private:
    Protocol CurrentProtocol = Protocol::NONE;
    std::string Server = "";
    uint16_t Port = 4567;
    uint16_t Timeout = 250;
    uint8_t RetransmitionNumber = 3;

    bool PrintHelp = false;
};

class ArgumentParser
{
public:
    static Arguments parseArguments(int argc, char** argv);
};