#pragma once 

#include "Message.hpp"

#include <string>
#include <arpa/inet.h>

class ConverterToString 
{
public:

    static std::string convertIP(struct addrinfo* ServerInfo);

    static std::string convertIP(struct in_addr Ip);

    static std::string convertPort(struct addrinfo* ServerInfo);

    static std::string convertPort(in_port_t Port);

    static std::string convertMessageType(MessageType Type);
};