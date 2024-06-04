#include "App.hpp"

#include "Config.hpp"
#include "Message.hpp"
#include "MessageController.hpp"
#include "IO.hpp"
#include "ErrorHandle.hpp"

#include <iostream>
#include <sys/epoll.h>
#include <csignal>
#include <functional>

App* App::AppPtr = nullptr;

App::App(Arguments Config)
{
    AppPtr = this;

    ErrorHandle::linkApp(this);

    Config::createConfig(Config);

    Connection = ConnectionCreator::createConnection();

    FSM = std::make_unique<ClientFSM>();

    signal(SIGINT, App::handleInterruptStatic);
}

App::~App()
{

}

void App::initEpoll()
{
    EpollFD = epoll_create1(0);
    if (EpollFD < 0) 
    {
        ErrorHandle::throwErrorAndExit("Cannot create Epoll", 99);
    }
    
    // Add UDP socket to epoll
    struct epoll_event SocketEvent;
    SocketEvent.events = EPOLLIN;
    SocketEvent.data.fd = Connection->getSocket();
    epoll_ctl(EpollFD, EPOLL_CTL_ADD, Connection->getSocket(), &SocketEvent);

    // Add stdin to epoll
    struct epoll_event UserInputEvent;
    UserInputEvent.events = EPOLLIN;
    UserInputEvent.data.fd = STDIN_FILENO;
    epoll_ctl(EpollFD, EPOLL_CTL_ADD, STDIN_FILENO, &UserInputEvent);

}

void App::freeEpoll()
{
    close(EpollFD);
}

void App::handleInterruptStatic(int SigNum)
{
    (void)SigNum;
    AppPtr->exitApp(0);
}

void App::exitApp(int ExitCode)
{
    stopConnection();

    freeEpoll();

    exit(ExitCode);
}

void App::startLoop()
{
    
    Connection->connectToServer();

    initEpoll();

    struct epoll_event Events[1];

    while (Running)
    {
        auto MessageHolder = MessageController::getMessageController()->getMessageHolder();

        if (MessageHolder->isEmpty() == false)
        {
            auto ReceivedMessage = MessageHolder->pop();
            proccessServerMessage(ReceivedMessage);

            continue;
        }

        epoll_wait(EpollFD, Events, 1, -1);

        int FileDescriptor = Events[0].data.fd;
        
        if (FileDescriptor == Connection->getSocket()) 
        {
            auto Message = Connection->receiveMessage();
            proccessServerMessage(Message);
        }
        else if (FileDescriptor == STDIN_FILENO) 
        {
            std::string UserLine = IO::getUserInput();
            auto Message = MessageCreator::createMessageFromUser(UserLine);
            processUserMessage(Message);
        }
    }
    
}

void App::proccessServerMessage(std::shared_ptr<IMessage> Message)
{
    ITextMessage* TEXT_MSG = nullptr;
    IReplyMessage* REPLY_MSG = nullptr;
    IErrorMessage* ERR_MSG = nullptr;

    switch (Message->getType())
    {
    case MessageType::NONE:
        return;

    case MessageType::UNDEFINED:
    case MessageType::JOIN:
    case MessageType::AUTH:
        Connection->sendMessage(MessageCreator::createError("Receive unexpected message"));
        ErrorHandle::throwErrorAndExit("Receive unexpected message", 12);
        FSM->pushServerInput(Message);
        break;

    case MessageType::CONFIRM:
        if (MessageController::getMessageController()->getUserProcessedMessage()->wasSended(Message->getID()) == false)
        {     
            Connection->sendMessage(MessageCreator::createError("Receive unexpected message ID"));
            ErrorHandle::throwErrorAndExit("Receive unexpected message ID", 12);
        }
        FSM->pushServerInput(Message);
        break;
        

    case MessageType::REPLY:
        if (MessageController::getMessageController()->getUserProcessedMessage()->wasWaitingReply(Message->getID()) == false)
        {     
            Connection->sendMessage(MessageCreator::createError("Receive reply with unexpected RefID"));
            ErrorHandle::throwErrorAndExit("Receive reply with unexpected RefID", 12);
        }
        if (MessageController::getMessageController()->getServerProcessedMessage()->isMessageProcessed(Message->getID()) == false)
        {
            REPLY_MSG = dynamic_cast<IReplyMessage*>(Message.get());
            IO::printReply(REPLY_MSG->getResult(), REPLY_MSG->getMessageContent());
            MessageController::getMessageController()->getServerProcessedMessage()->addProcessedMessage(Message->getID());
            FSM->pushServerInput(Message);
        }
        break;


    case MessageType::TEXT_MSG:
        if (MessageController::getMessageController()->getServerProcessedMessage()->isMessageProcessed(Message->getID()) == false)
        {
            TEXT_MSG = dynamic_cast<ITextMessage*>(Message.get());
            IO::printMessage(TEXT_MSG->getNickname(), TEXT_MSG->getMessageContent());
            MessageController::getMessageController()->getServerProcessedMessage()->addProcessedMessage(Message->getID());
            FSM->pushServerInput(Message);
        }
        break;

    case MessageType::ERROR:
        ERR_MSG = dynamic_cast<IErrorMessage*>(Message.get());
        IO::printServerError(ERR_MSG->getNickname(), ERR_MSG->getMessageContent());
        this->exitApp(0);
        break;

    case MessageType::BYE:
        Running = false;
        break;
    }
}

void App::processUserMessage(std::shared_ptr<IMessage> Message)
{
    bool Result = false;
    switch (Message->getType())
    {
    case MessageType::NONE:
        break;

    case MessageType::UNDEFINED:
    case MessageType::CONFIRM:
    case MessageType::REPLY:
    case MessageType::ERROR:
    case MessageType::BYE:
        ErrorHandle::throwErrorAndExit("Receive unexpected message from user", 10);
        break;

    case MessageType::JOIN:
        if (FSM->isUserInputSuitable(Message) == false)
        {
            IO::printInternalError("Can't join channel currently");
            return;
        }

        Result = Connection->sendMessage(Message);
        if (Result == false)
        {
            ErrorHandle::throwErrorAndExit("Error with sending message", 12);
        }
        MessageController::getMessageController()->getUserProcessedMessage()->pushMessageThatWantReply(Message->getID());
        FSM->pushUserInput(Message);
        break;

    case MessageType::AUTH:
        if (FSM->isUserInputSuitable(Message) == false)
        {
            IO::printInternalError("Can't authorize currently");
            return;
        }

        Result = Connection->sendMessage(Message);
        if (Result == false)
        {
            ErrorHandle::throwErrorAndExit("Error with sending message", 12);
        }
        MessageController::getMessageController()->getUserProcessedMessage()->pushMessageThatWantReply(Message->getID());
        FSM->pushUserInput(Message);
        break;

    case MessageType::TEXT_MSG:
        if (FSM->isUserInputSuitable(Message) == false)
        {
            IO::printInternalError("Can't send message currently");
            return;
        }
        Result = Connection->sendMessage(Message);
        if (Result == false)
        {
            ErrorHandle::throwErrorAndExit("Error with sending message", 12);
        }
        MessageController::getMessageController()->getUserProcessedMessage()->pushSendedMessage(Message->getID());
        FSM->pushUserInput(Message);
        break;
    }
}

void App::stopConnection()
{
    if (Connection->isConnected() == true)
    {
        auto Message = MessageCreator::createByeMessage();
        this->Connection->sendMessage(Message);
        this->Connection->stopConnection();
    }

}
