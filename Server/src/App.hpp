#pragma once

#include "Arguments.hpp"
#include "User.hpp"
#include "WelcomePoint.hpp"
#include "ChannelHolder.hpp"
#include "AuthenticationHandler.hpp"

class App
{
public:
    App(Arguments Args);
    
    ~App();

    void start();

    void raiseError(std::string Error);

private:
    void setupWelcome();

    void mainLoop();

    void bindToEpoll(int Socket);

    void unbindFromEpoll(int Socket);

private:
    std::vector<std::shared_ptr<User>> Users;

    std::unique_ptr<WelcomePoint> WelcomeConnectionUDP;

    std::unique_ptr<WelcomePoint> WelcomeConnectionTCP;

    std::unique_ptr<ChannelHolder> Channels;

    std::unique_ptr<AuthenticationHandler> Authentication;

    bool Running = true;

    int EpollFD;

};