#pragma once 

#include <string>

class IO
{
public:
    static void printSendMessage(std::string IP, std::string Port, std::string MessageType);
    
    static void printReceiveMessage(std::string IP, std::string Port, std::string MessageType);
};