#pragma once

#include "Message.hpp"

class ClientFSM;

class IClientState
{
public:
    IClientState();

    virtual ~IClientState() {};

    virtual bool isUserInputSuitable(std::shared_ptr<IMessage> Message) = 0;

    virtual void pushUserInput(std::shared_ptr<IMessage> Message) = 0;

    virtual void pushServerInput(std::shared_ptr<IMessage> Message) = 0;

    virtual bool isError() = 0;

    void setFSM(ClientFSM* FSM);

protected:
    ClientFSM* FSM;
};

class StartState : public IClientState
{
public:
    bool isUserInputSuitable(std::shared_ptr<IMessage> Message);

    void pushUserInput(std::shared_ptr<IMessage> Message);

    void pushServerInput(std::shared_ptr<IMessage> Message);

    bool isError() { return false; }
};

class AuthWaitState : public IClientState
{
public:
    bool isUserInputSuitable(std::shared_ptr<IMessage> Message);

    void pushUserInput(std::shared_ptr<IMessage> Message);

    void pushServerInput(std::shared_ptr<IMessage> Message);

    bool isError() { return false; }

};

class OpenState : public IClientState
{
public:
    bool isUserInputSuitable(std::shared_ptr<IMessage> Message);

    void pushUserInput(std::shared_ptr<IMessage> Message);

    void pushServerInput(std::shared_ptr<IMessage> Message);

    bool isError() { return false; }
};

class ErrorState : public IClientState
{
public:
    bool isUserInputSuitable(std::shared_ptr<IMessage> Message);

    void pushUserInput(std::shared_ptr<IMessage> Message);

    void pushServerInput(std::shared_ptr<IMessage> Message);
    
    bool isError() { return true; }

};

class EndState : public IClientState
{
public:
    bool isUserInputSuitable(std::shared_ptr<IMessage> Message);

    void pushUserInput(std::shared_ptr<IMessage> Message);

    void pushServerInput(std::shared_ptr<IMessage> Message);

    bool isError() { return false; }

};
