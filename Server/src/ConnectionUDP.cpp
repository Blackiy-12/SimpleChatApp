#include "ConnectionUDP.hpp"

#include "Config.hpp"
#include "Converter.hpp"
#include "Error.hpp"
#include "IO.hpp"

#include <cstring>
#include <errno.h>


ConnectionUDP::ConnectionUDP(std::string ServerAddress, uint16_t ServerPort)
{
    Buffer.resize(BufferMaxSize);
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    char PortChar[10];
    sprintf(PortChar, "%u", ServerPort);
    
    int status = getaddrinfo(ServerAddress.c_str(), PortChar, &hints, &ServerInfo);

    if (status != 0 || ServerInfo->ai_addr == NULL)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not get address of server");
    }

    getaddrinfo(ServerAddress.c_str(), PortChar, &hints, &ClientInfo);
}

ConnectionUDP::ConnectionUDP(struct addrinfo* ClientInfo)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    std::string IP = ConverterToString::convertIP(ClientInfo);
    auto PortString = ConverterToString::convertPort(ClientInfo);

    int status = getaddrinfo(IP.c_str(), PortString.c_str(), &hints, &this->ClientInfo);

    if (status != 0 || ClientInfo->ai_addr == NULL)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not get address of server");
    }

    getaddrinfo(IP.c_str(), PortString.c_str(), &hints, &this->ServerInfo);
}

ConnectionUDP::~ConnectionUDP()
{
    freeaddrinfo(ServerInfo);
    freeaddrinfo(ClientInfo);
    close(Socket);
}

void ConnectionUDP::createSocket()
{
    Socket = socket(ServerInfo->ai_family, ServerInfo->ai_socktype, ServerInfo->ai_protocol);

    if (Socket < 0) 
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not create socket");
    }

    int optval = 1;
    if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) 
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not set up socket");
    }

    struct timeval Timeout;
    Timeout.tv_sec = 0;
    Timeout.tv_usec = Config::getConfig()->getUDPTimeout() * 1000;

    if(setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&Timeout, sizeof(Timeout)) < 0)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not set up socket");
    }
}

void ConnectionUDP::acceptConnection(int WelcomeSocket)
{
    (void)WelcomeSocket;
}

void ConnectionUDP::bindSocket()
{
    if(bind(Socket, ServerInfo->ai_addr, ServerInfo->ai_addrlen) == -1)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can't bind socket");
    }
}

struct addrinfo* ConnectionUDP::getLastAddress()
{
    return ClientInfo;
}

bool ConnectionUDP::sendMessage(std::shared_ptr<IMessage> Message)
{
    Buffer.clear();
    Buffer = Message->getMessageForSent();
    
    int send_byte = sendto(Socket, Buffer.data(), Buffer.size(), 0, ClientInfo->ai_addr, ClientInfo->ai_addrlen);
    if (send_byte < 0)
    {
        return false;
    }

    auto IP = ConverterToString::convertIP(ClientInfo);
    auto Port = ConverterToString::convertPort(ClientInfo);
    auto Type = ConverterToString::convertMessageType(Message->getType());
    IO::printSendMessage(IP, Port, Type);


    if (Message->getType() == MessageType::CONFIRM)
    {
        return true;
    }

    //wait 250ms 
    Buffer.clear();
    Buffer.resize(BufferMaxSize);
    int recv_byte = recvfrom(Socket, Buffer.data(), Buffer.size(), 0, ClientInfo->ai_addr, &ClientInfo->ai_addrlen);
    
    if (recv_byte < 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return confirmSend(Message);
        }
        else
        {
            return false;
        }
    }
    
    std::vector<uint8_t> ReceivedBytes;
    std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));
    
    auto ReceivedMessage = MessageCreator::translateMessage(Protocol::UDP, ReceivedBytes);

    
    IP = ConverterToString::convertIP(ClientInfo);
    Port = ConverterToString::convertPort(ClientInfo);
    Type = ConverterToString::convertMessageType(ReceivedMessage->getType());
    IO::printReceiveMessage(IP, Port, Type);

    if (ReceivedMessage->getType() != MessageType::CONFIRM)
    {
        MessageBuffer->addMessage(ReceivedMessage);
        auto ConfMessage = MessageCreator::createMessageToConfirm(ReceivedMessage->getID());
        sendMessage(ConfMessage);
    }
    else
    {
        if (ReceivedMessage->getID() == Message->getID())
            return true;
    }
    
    return confirmSend(Message);
}

void ConnectionUDP::receiveMessage()
{
    Buffer.clear();
    Buffer.resize(BufferMaxSize);
    int recv_byte = recvfrom(Socket, Buffer.data(), Buffer.size(), 0, ClientInfo->ai_addr, &ClientInfo->ai_addrlen);
    
    if (recv_byte < 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return;
        }
        else
        {
            ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Error while receiving data");
        }
    }
    else if (recv_byte == 0)
    {
        return;
    }

    std::vector<uint8_t> ReceivedBytes;
    std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));

    auto MSG = MessageCreator::translateMessage(Protocol::UDP, ReceivedBytes);

    auto IP = ConverterToString::convertIP(ClientInfo);
    auto Port = ConverterToString::convertPort(ClientInfo);
    auto Type = ConverterToString::convertMessageType(MSG->getType());
    IO::printReceiveMessage(IP, Port, Type);

    MessageBuffer->addMessage(MSG);

    switch (MSG->getType())
    {
    case MessageType::CONFIRM:
    case MessageType::UNDEFINED:
    case MessageType::NONE:
        return;
    
    default:
        break;
    }

    auto ConfirmMessage = MessageCreator::createMessageToConfirm(MSG->getID());
    sendMessage(ConfirmMessage);
    return;
}

bool  ConnectionUDP::confirmSend(std::shared_ptr<IMessage> Message)
{
    for (int i = 0; i < Config::getConfig()->getNumberOfRetransmition(); i++)
    {

        Buffer.clear();
        Buffer = Message->getMessageForSent();

        int send_byte = sendto(Socket, Buffer.data(), Buffer.size(), 0, ClientInfo->ai_addr, ClientInfo->ai_addrlen);

        auto IP = ConverterToString::convertIP(ClientInfo);
        auto Port = ConverterToString::convertPort(ClientInfo);
        auto Type = ConverterToString::convertMessageType(Message->getType());
        IO::printSendMessage(IP, Port, Type);

        
        if (send_byte < 0)
        {
            ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Error while sending data");
        }

        Buffer.clear();
        Buffer.resize(BufferMaxSize);
        int recv_byte = recvfrom(Socket, Buffer.data(), Buffer.size(), 0, ServerInfo->ai_addr, &ServerInfo->ai_addrlen);

        if (recv_byte < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                continue;
            }
            else
            {
                ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Error while receiving data");
            }
        }
        else if (recv_byte  == 0)
        {
            continue;
        }

        std::vector<uint8_t> ReceivedBytes;
        std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));

        auto ReceivedMessage = MessageCreator::translateMessage(Protocol::UDP, ReceivedBytes);
        
        IP = ConverterToString::convertIP(ClientInfo);
        Port = ConverterToString::convertPort(ClientInfo);
        Type = ConverterToString::convertMessageType(ReceivedMessage->getType());
        IO::printReceiveMessage(IP, Port, Type);

        if (ReceivedMessage->getType() != MessageType::CONFIRM)
        {

            MessageBuffer->addMessage(ReceivedMessage);
            auto ConfMessage = MessageCreator::createMessageToConfirm(ReceivedMessage->getID());
            sendMessage(ConfMessage);
        }
        else
        {
            if (ReceivedMessage->getID() == Message->getID())
                return true;
        }
    }

    return false;
}

void ConnectionUDP::linkMessageBuffer(IMessageBuffer* Buffer)
{
    MessageBuffer = Buffer;
}

std::string ConnectionUDP::getIP()
{
    return ConverterToString::convertIP(ClientInfo);
}

std::string ConnectionUDP::getPort()
{
    return ConverterToString::convertPort(ClientInfo);
}