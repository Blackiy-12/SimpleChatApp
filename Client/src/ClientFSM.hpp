#pragma once 

#include <memory>
#include "Message.hpp"

class IClientState;

class ClientFSM
{
public:
    ClientFSM();

    ~ClientFSM();

    bool isUserInputSuitable(std::shared_ptr<IMessage> Message);

    void pushUserInput(std::shared_ptr<IMessage> Message);

    void pushServerInput(std::shared_ptr<IMessage> Message);

    bool isInError();

    void setNewState(IClientState* State);

private:
    IClientState* CurrentState;
};
