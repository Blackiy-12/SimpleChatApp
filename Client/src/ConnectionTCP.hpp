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

    ~ConnectionTCP();

    int getSocket() { return Socket; }

    void connectToServer();

    void stopConnection();

    bool sendMessage(std::shared_ptr<IMessage> Message);

    std::shared_ptr<IMessage> receiveMessage();

    bool isConnected() const { return Connected; }

private:
    bool Connected = false;

    std::string HostName;

    uint16_t Port;

    int Socket = -1;

    struct addrinfo* ServerInfo;

    std::vector<uint8_t> Buffer {};

    const int BufferMaxSize = 1435;

};