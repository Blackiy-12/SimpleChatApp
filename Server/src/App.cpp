#include "App.hpp"

#include "Config.hpp"
#include "Error.hpp"

#include <sys/epoll.h>

App::App(Arguments Args)
{
    Config::getConfig()->createConfig(Args);
    ErrorHandler::linkApp(this);
}

App::~App()
{
    
}

void App::start()
{
    EpollFD = epoll_create1(0);
    if (EpollFD == -1)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_EPOLL, "Can't bind event to epoll");
    }

    Channels = std::make_unique<ChannelHolder>();

    Authentication = std::make_unique<AuthenticationHandler>();
    
    setupWelcome();

    mainLoop();
}

void App::raiseError(std::string Error)
{
    for (auto& User : Users)
    {
        User->endWithError(Error);
    }
}

void App::bindToEpoll(int Socket)
{
    struct epoll_event Event;
    Event.events = EPOLLIN;
    Event.data.fd = Socket;
    if (epoll_ctl(EpollFD, EPOLL_CTL_ADD, Socket, &Event) == -1) 
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_EPOLL, "Can't bind event to epoll");
    }
}

void App::unbindFromEpoll(int Socket)
{
    if (epoll_ctl(EpollFD, EPOLL_CTL_DEL, Socket, NULL) == -1) 
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_EPOLL, "Can't delete event from epoll");
    }
}

void App::setupWelcome()
{
    auto IP = Config::getConfig()->getServer();
    auto Port = Config::getConfig()->getServerPort();

    WelcomeConnectionTCP = std::make_unique<WelcomePoint>();
    WelcomeConnectionTCP->createConnection(Protocol::TCP, IP, Port);
    WelcomeConnectionTCP->linkToAuthenticationHandler(Authentication.get());
    
    WelcomeConnectionUDP = std::make_unique<WelcomePoint>();
    WelcomeConnectionUDP->createConnection(Protocol::UDP, IP, Port);
    WelcomeConnectionUDP->linkToAuthenticationHandler(Authentication.get());
}

void App::mainLoop()
{
    bindToEpoll(WelcomeConnectionTCP->getSocket());

    bindToEpoll(WelcomeConnectionUDP->getSocket());

    struct epoll_event Events[1];
    while (Running)
    {
        int Result = epoll_wait(EpollFD, Events, 1, -1);

        if (Result < 0 && errno == 4)
        {
            continue;
        }

        if (Result < 0)
        {
            ErrorHandler::throwError(ErrorCode::ERROR_WITH_EPOLL, "Error with waiting epoll event");
        }

        if (Events[0].data.fd == WelcomeConnectionTCP->getSocket())
        {
            auto NewUser = WelcomeConnectionTCP->processNewUser(Channels.get());
            if (NewUser != nullptr)
            {
                bindToEpoll(NewUser->getUserSocket());
                Users.push_back(NewUser);
            }
        }
        else if (Events[0].data.fd == WelcomeConnectionUDP->getSocket())
        {
            auto NewUser = WelcomeConnectionUDP->processNewUser(Channels.get());
            if (NewUser != nullptr)
            {
                bindToEpoll(NewUser->getUserSocket());
                Users.push_back(NewUser);
            }
        }
        else
        {
            for(auto &User : Users)
            {
                if (Events[0].data.fd == User->getUserSocket())
                {
                    User->receiveMessage();

                    if (User->getCurrentState() == UserState::BYE || User->getCurrentState() == UserState::ERR)
                    {
                        User->sendBye();

                        auto ToDelete = std::find(Users.begin(), Users.end(), User);
                        Users.erase(Users.begin() + std::distance(Users.begin(), ToDelete));
                    }
                    break;
                }
            }
        }
    }
}