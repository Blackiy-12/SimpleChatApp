#include "Converter.hpp"

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

std::string ConverterToString::convertIP(struct addrinfo* ServerInfo)
{
    char IPChr[INET_ADDRSTRLEN];
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)ServerInfo->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), IPChr, INET_ADDRSTRLEN);//TODO maybe another function(inet_ntoa)

    return std::string(IPChr);
}

std::string ConverterToString::convertIP(struct in_addr Ip)
{
    return std::string(inet_ntoa(Ip));
}

std::string ConverterToString::convertPort(struct addrinfo* ServerInfo)
{
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)ServerInfo->ai_addr;
    auto Port = ntohs(ipv4->sin_port);
    return std::to_string(Port);
}

std::string ConverterToString::convertPort(in_port_t Port)
{
    return std::to_string(ntohs(Port));
}

std::string ConverterToString::convertMessageType(MessageType Type)
{
    switch (Type)
    {
    case MessageType::NONE:
        return std::string("NONE");

    case MessageType::UNDEFINED:
        return std::string("UNDEFINED");

    case MessageType::CONFIRM:
        return std::string("CONFIRM");

    case MessageType::AUTH:
        return std::string("AUTH");

    case MessageType::REPLY:
        return std::string("REPLY");

    case MessageType::JOIN:
        return std::string("JOIN");

    case MessageType::TEXT_MSG:
        return std::string("MSG");

    case MessageType::ERROR:
        return std::string("ERR");

    case MessageType::BYE:
        return std::string("BYE");
    }

    return std::string("");
}