#pragma once

#include "Connection.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>

class ConnectionUDP : public IConnection
{
public:
    ConnectionUDP(std::string ServerAddress, uint16_t ServerPort);
    
    ConnectionUDP(struct addrinfo* ClientInfo);
    
    ~ConnectionUDP();

    void createSocket();

    void acceptConnection(int WelcomeSocket);

    void bindSocket();

    struct addrinfo* getLastAddress();

    int getSocket() { return Socket; };

    bool sendMessage(std::shared_ptr<IMessage> Message);

    void receiveMessage();
    
    void linkMessageBuffer(IMessageBuffer* Buffer);
    
    std::string getIP();

    std::string getPort();

private:
    bool confirmSend(std::shared_ptr<IMessage> Message);

private:
    int Socket = -1;

    struct addrinfo* ServerInfo = nullptr;

    struct addrinfo* ClientInfo = nullptr;

    std::vector<uint8_t> Buffer {};

    const int BufferMaxSize = 1425;

    IMessageBuffer* MessageBuffer;

};
