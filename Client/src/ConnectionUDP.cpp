#include "ConnectionUDP.hpp"

#include <cstring>
#include <errno.h>

#include "ErrorHandle.hpp"
#include "Config.hpp"
#include "MessageController.hpp"

ConnectionUDP::ConnectionUDP(std::string ServerAddress, uint16_t ServerPort)
:   Connected(false), HostName(ServerAddress), Port(ServerPort)
{
    Buffer.resize(BufferMaxSize);
}

ConnectionUDP::~ConnectionUDP()
{
    freeaddrinfo(ServerInfo);
    close(Socket);
}

void ConnectionUDP::connectToServer()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

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


    struct timeval Timeout;
    Timeout.tv_sec = 0;
    Timeout.tv_usec = Config::getConfig()->getUDPTimeout() * 1000;

    if(setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&Timeout, sizeof(Timeout)) < 0)
    {
        ErrorHandle::throwErrorAndExit("Can not set up socket", 99);
    }

    Connected = true;
}

void ConnectionUDP::stopConnection()
{
    Connected = false;
}

bool ConnectionUDP::sendMessage(std::shared_ptr<IMessage> Message)
{
    Buffer.clear();
    Buffer = Message->getMessageForSent();


    int send_byte = sendto(Socket, Buffer.data(), Buffer.size(), 0, ServerInfo->ai_addr, ServerInfo->ai_addrlen);
    if (send_byte < 0)
    {
        ErrorHandle::throwErrorAndExit("Error while sending data", 12);
    }

    if (Message->getType() == MessageType::CONFIRM)
    {
        return true;
    }

    //wait 250ms 
    Buffer.clear();
    Buffer.resize(BufferMaxSize);
    int recv_byte = recvfrom(Socket, Buffer.data(), Buffer.size(), 0, ServerInfo->ai_addr, &ServerInfo->ai_addrlen);
    
    if (recv_byte < 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return confirmSend(Message);
        }
        else
        {
            ErrorHandle::throwErrorAndExit("Error while receiving data", 12);
        }
    }
    
    std::vector<uint8_t> ReceivedBytes;
    std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));
    
    auto ReceivedMessage = MessageCreator::translateMessageFromServer(ReceivedBytes);

    switch (ReceivedMessage->getType())
    {
    case MessageType::CONFIRM:

        if (ReceivedMessage->getID() == Message->getID())
            return true;
        else 
            MessageController::getMessageController()->getMessageHolder()->push(ReceivedMessage);

        break;

    case MessageType::NONE:
    case MessageType::UNDEFINED:
        MessageController::getMessageController()->getMessageHolder()->push(ReceivedMessage);
        break;
    
    default:
        MessageController::getMessageController()->getMessageHolder()->push(ReceivedMessage);
        sendMessage(MessageCreator::createMessageToConfirm(ReceivedMessage->getID()));
        break;
    }

    return confirmSend(Message);
}

std::shared_ptr<IMessage> ConnectionUDP::receiveMessage()
{
    Buffer.clear();
    Buffer.resize(BufferMaxSize);
    int recv_byte = recvfrom(Socket, Buffer.data(), Buffer.size(), 0, ServerInfo->ai_addr, &ServerInfo->ai_addrlen);
    
    if (recv_byte < 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            return std::make_shared<NoneMessage>();
        }
        else
        {
            ErrorHandle::throwErrorAndExit("Error while receiving data", 12);
        }
    }
    else if (recv_byte == 0)
    {
        return std::make_shared<NoneMessage>();
    }

    std::vector<uint8_t> ReceivedBytes;
    std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));

    auto MSG = MessageCreator::translateMessageFromServer(ReceivedBytes);

    switch (MSG->getType())
    {
    case MessageType::CONFIRM:
    case MessageType::UNDEFINED:
    case MessageType::NONE:
        return MSG;
    
    default:
        break;
    }

    auto ConfirmMessage = MessageCreator::createMessageToConfirm(MSG->getID());
    sendMessage(ConfirmMessage);
    return MSG;
}

bool  ConnectionUDP::confirmSend(std::shared_ptr<IMessage> Message)
{
    for (int i = 0; i < Config::getConfig()->getNumberOfRetransmition(); i++)
    {

        Buffer.clear();
        Buffer = Message->getMessageForSent();

        int send_byte = sendto(Socket, Buffer.data(), Buffer.size(), 0, ServerInfo->ai_addr, ServerInfo->ai_addrlen);
        
        if (send_byte < 0)
        {
            ErrorHandle::throwErrorAndExit("Error while sending data", 12);
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
                ErrorHandle::throwErrorAndExit("Error while receiving data", 12);
            }
        }

        std::vector<uint8_t> ReceivedBytes;
        std::copy(Buffer.begin(), Buffer.begin() + recv_byte, std::back_inserter(ReceivedBytes));

        auto ReceivedMessage = MessageCreator::translateMessageFromServer(ReceivedBytes);

        switch (ReceivedMessage->getType())
        {
        case MessageType::CONFIRM:
            return true;
    
        case MessageType::NONE:
        case MessageType::UNDEFINED:
            MessageController::getMessageController()->getMessageHolder()->push(ReceivedMessage);
            break;
        
        default:
            MessageController::getMessageController()->getMessageHolder()->push(ReceivedMessage);
            sendMessage(MessageCreator::createMessageToConfirm(ReceivedMessage->getID()));
            break;
        }
    }

    return false;
}

