#pragma once
#include <string>
#include <cstdint>
#include "Protocol.hpp"

class Arguments
{
public:
    Arguments() {};
    ~Arguments() {};

    void printHelpInfo();

    //Setters
    void setServer(std::string NewServer);
    void setServerPort(uint16_t NewPort);
    void setUDPTimeout(uint16_t Time);
    void setNumberOfRetransmition(uint8_t NewNumber);
    void setHelpArgument();

    //Getters
    std::string getServer() const;
    uint16_t getServerPort() const;
    uint16_t getUDPTimeout() const;
    uint8_t getNumberOfRetransmition() const;
    bool needToPrintHelp();
    
private:
    std::string Server = "0.0.0.0";
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