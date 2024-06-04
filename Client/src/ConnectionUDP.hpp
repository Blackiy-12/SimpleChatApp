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
    
    ~ConnectionUDP();

    int getSocket() { return Socket; }

    void connectToServer();

    void stopConnection();

    bool sendMessage(std::shared_ptr<IMessage> Message);

    std::shared_ptr<IMessage> receiveMessage() override;

    bool isConnected() const { return Connected; }

private:
    bool confirmSend(std::shared_ptr<IMessage> Message);

private:
    bool Connected;

    std::string HostName;

    uint16_t Port;

    int Socket = -1;

    struct addrinfo* ServerInfo;

    std::vector<uint8_t> Buffer {};

    const int BufferMaxSize = 1425;

};
