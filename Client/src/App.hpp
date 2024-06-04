#pragma once

#include "Arguments.hpp"
#include "Connection.hpp"
#include "ClientFSM.hpp"

class App
{
public:
    App(Arguments Config);

    ~App();

    void startLoop();

    static void handleInterruptStatic(int SigNum);
    
    void exitApp(int ExitCode);
    
private:
    bool Running = true;

    std::shared_ptr<IConnection> Connection;

    std::unique_ptr<ClientFSM> FSM;

    int EpollFD = -1;

    static App* AppPtr;

private:
    void proccessServerMessage(std::shared_ptr<IMessage> Message);

    void processUserMessage(std::shared_ptr<IMessage> Message);

    void stopConnection();

    void initEpoll();

    void freeEpoll();


};