#include "ConnectionTCP.hpp"

#include <cstring>
#include <errno.h>

#include "ErrorHandle.hpp"
#include "Config.hpp"
#include "MessageController.hpp"

ConnectionTCP::ConnectionTCP(std::string ServerAddress, uint16_t ServerPort)
:   HostName(ServerAddress), Port(ServerPort)
{
    Buffer.resize(BufferMaxSize);
}

ConnectionTCP::~ConnectionTCP()
{
    freeaddrinfo(ServerInfo);
    close(Socket);
}

void ConnectionTCP::connectToServer()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char PortChar[10];
    sprintf(PortChar, "%u", Port);
    
    int status = getaddrinfo(HostName.c_str(), PortChar, &hints, &ServerInfo);

    if (status != 0 || ServerInfo->ai_addr == NULL)
    {
        ErrorHandle::throwErrorAndExit("Can not get address of server", 99);
    }
    
    Socket = socket(ServerInfo->ai_family, ServerInfo->ai_socktype, ServerInfo->ai_protocol);

    if (Socket < 0) 
    {
        ErrorHandle::throwErrorAndExit("Can not create socket", 99);
    }

    if (connect(Socket, ServerInfo->ai_addr, ServerInfo->ai_addrlen) == -1)
    {
        ErrorHandle::throwErrorAndExit("Can not connect to server", 11);
    }

    Connected = true;

}

void ConnectionTCP::stopConnection()
{
    Connected = false;
}

bool ConnectionTCP::sendMessage(std::shared_ptr<IMessage> Message)
{
    Buffer.clear();
    Buffer = Message->getMessageForSent();

    int send_byte = send(Socket, Buffer.data(), Buffer.size(), 0);
    if (send_byte < 0) 
    {
        ErrorHandle::throwErrorAndExit("Error while sending data", 12);  
        return false;
    }

    return true;
}

std::shared_ptr<IMessage> ConnectionTCP::receiveMessage()
{
    Buffer.clear();
    Buffer.resize(BufferMaxSize);

    int recv_byte = recv(Socket, Buffer.data(), Buffer.size(), 0);
    if (recv_byte <= 0)
    {
        ErrorHandle::throwErrorAndExit("Error while sending data", 12);  
    }

    std::vector<uint8_t> ReceivedBytes;
    std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));
    
    auto MSG = MessageCreator::translateMessageFromServer(ReceivedBytes);
    return MSG;
}
