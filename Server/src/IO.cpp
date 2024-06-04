#include "IO.hpp"

#include <iostream>

void IO::printSendMessage(std::string IP, std::string Port, std::string MessageType)
{
    std::cout << "SENT " << IP << ":" << Port << " | " << MessageType << "\n";
}

void IO::printReceiveMessage(std::string IP, std::string Port, std::string MessageType)
{
    std::cout << "RECV " << IP << ":" << Port << " | " << MessageType << "\n";
}