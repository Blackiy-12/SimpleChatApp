#include "ConnectionTCP.hpp"

#include "Error.hpp"
#include "IO.hpp"
#include "Converter.hpp"

#include <cstring>
#include <errno.h>
#include <sys/socket.h>

ConnectionTCP::ConnectionTCP(std::string ServerAddress, uint16_t ServerPort)
{
    Buffer.resize(BufferMaxSize);
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char PortChar[10];
    sprintf(PortChar, "%u", ServerPort);
    
    int status = getaddrinfo(ServerAddress.c_str(), PortChar, &hints, &ServerInfo);

    if (status != 0 || ServerInfo->ai_addr == NULL)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not get address of server");
    }
}

ConnectionTCP::ConnectionTCP(struct addrinfo* ClientInfo)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    std::string IP = ConverterToString::convertIP(ClientInfo);
    auto PortString = ConverterToString::convertPort(ClientInfo);

    int status = getaddrinfo(IP.c_str(), PortString.c_str(), &hints, &this->ServerInfo);

    if (status != 0 || ClientInfo->ai_addr == NULL)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can not get address of server");
    }
}

ConnectionTCP::~ConnectionTCP()
{
    freeaddrinfo(ServerInfo);
    close(Socket);
}

void ConnectionTCP::createSocket()
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

}

void ConnectionTCP::acceptConnection(int WelcomeSocket)
{
    ClientSize = sizeof(ClientInfo);
    Socket = accept(WelcomeSocket, (struct sockaddr *)&ClientInfo, &ClientSize);
}

void ConnectionTCP::bindSocket()
{
    if(bind(Socket, ServerInfo->ai_addr, ServerInfo->ai_addrlen) == -1)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can't bind socket");
    }

    if(listen(Socket, 1) == -1)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Can't listen socket");
    }
}

struct addrinfo* ConnectionTCP::getLastAddress()
{
    return ServerInfo;
}

bool ConnectionTCP::sendMessage(std::shared_ptr<IMessage> Message)
{
    Buffer.clear();
    Buffer = Message->getMessageForSent();

    int send_byte = send(Socket, Buffer.data(), Buffer.size(), 0);

    auto IP = ConverterToString::convertIP(ClientInfo.sin_addr);
    auto Port = ConverterToString::convertPort(ClientInfo.sin_port);
    auto Type = ConverterToString::convertMessageType(Message->getType());
    IO::printSendMessage(IP, Port, Type);

    if (send_byte < 0) 
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Error while sending data");
    }

    return true;
}

void ConnectionTCP::receiveMessage()
{
    Buffer.clear();
    Buffer.resize(BufferMaxSize);

    int recv_byte = recv(Socket, Buffer.data(), Buffer.size(), 0);
    if (recv_byte < 0)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Error while sending data");
    }

    std::vector<uint8_t> ReceivedBytes;
    std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));

    MessageBuffer->addMessage(ReceivedBytes);

    return;
}

void ConnectionTCP::linkMessageBuffer(IMessageBuffer* Buffer)
{
    MessageBuffer = Buffer;
}


std::string ConnectionTCP::getIP()
{
    return ConverterToString::convertIP(ClientInfo.sin_addr);
}

std::string ConnectionTCP::getPort()
{
    return ConverterToString::convertPort(ClientInfo.sin_port);
}
