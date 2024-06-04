#include "Arguments.hpp"

#include <cstring>
#include <iostream>

void Arguments::printHelpInfo()
{
    std::cout << "Messenger server \n";
    std::cout << "Available arguments \n";
    std::cout << "-s address - address of server (standard 0.0.0.0) \n";
    std::cout << "-p port - port of server (standard 4567) \n";
    std::cout << "-d time - UDP confirmation timeout in ms (standard 250 ms)\n";
    std::cout << "-r number - Maximum number of UDP retransmissions (standard 3)\n";
    std::cout << "-h - print this message \n";
}

void Arguments::setServer(std::string NewServer)
{
    Server = NewServer;
}

void Arguments::setServerPort(uint16_t NewPort)
{
    Port = NewPort;
}

void Arguments::setUDPTimeout(uint16_t Time)
{
    Timeout = Time;
}

void Arguments::setNumberOfRetransmition(uint8_t NewNumber)
{
    RetransmitionNumber = NewNumber;
}

void Arguments::setHelpArgument()
{
    PrintHelp = true;
}

std::string Arguments::getServer() const
{
    return Server;
}

uint16_t Arguments::getServerPort() const
{
    return Port;
}

uint16_t Arguments::getUDPTimeout() const
{
    return Timeout;
}

uint8_t Arguments::getNumberOfRetransmition() const
{
    return RetransmitionNumber;
}

bool Arguments::needToPrintHelp() 
{
    return PrintHelp;
}


Arguments ArgumentParser::parseArguments(int argc, char** argv)
{
    Arguments Args;
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0)
        {
            i++;
            Args.setServer(std::string(argv[i]));

        }
        else if(strcmp(argv[i], "-p") == 0)
        {
            i++;
            Args.setServerPort(uint16_t(atoi(argv[i])));
            
        }
        else if(strcmp(argv[i], "-d") == 0)
        {
            i++;
            Args.setUDPTimeout(uint16_t(atoi(argv[i])));
            
        }
        else if(strcmp(argv[i], "-r") == 0)
        {
            i++;
            Args.setNumberOfRetransmition(uint8_t(atoi(argv[i])));
            
        }
        else if(strcmp(argv[i], "-h") == 0)
        {
            Args.setHelpArgument();
        }
        else
        {
            exit(99);
        }
    }

    return Args;
}