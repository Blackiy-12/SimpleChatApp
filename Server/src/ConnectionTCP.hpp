#pragma once
#include "Connection.hpp"

#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>

class ConnectionTCP: public IConnection
{
public:
    ConnectionTCP(std::string ServerAddress, uint16_t ServerPort);

    ConnectionTCP(struct addrinfo* ClientInfo);

    ~ConnectionTCP();

    void createSocket();

    void acceptConnection(int WelcomeSocket);

    void bindSocket();

    struct addrinfo* getLastAddress();

    int getSocket() { return Socket; }

    bool sendMessage(std::shared_ptr<IMessage> Message);

    void receiveMessage();
    
    void linkMessageBuffer(IMessageBuffer* Buffer);

    std::string getIP();

    std::string getPort();

private:
    int Socket = -1;

    struct addrinfo* ServerInfo = nullptr;
    
    struct sockaddr_in ClientInfo;
   
    socklen_t ClientSize;

    std::vector<uint8_t> Buffer {};

    const int BufferMaxSize = 1435;

    IMessageBuffer* MessageBuffer;

};