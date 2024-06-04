#include "ClientFSM.hpp"

#include "ClientState.hpp"

ClientFSM::ClientFSM()
{
    CurrentState = new StartState();
    CurrentState->setFSM(this);
}

ClientFSM::~ClientFSM()
{
    delete CurrentState;
}

bool ClientFSM::isUserInputSuitable(std::shared_ptr<IMessage> Message)
{
    return CurrentState->isUserInputSuitable(Message);
}

void ClientFSM::pushUserInput(std::shared_ptr<IMessage> Message)
{  
    CurrentState->pushUserInput(Message);
}

void ClientFSM::pushServerInput(std::shared_ptr<IMessage> Message)
{
    CurrentState->pushServerInput(Message);
}

bool ClientFSM::isInError()
{
    return CurrentState->isError();
}

void ClientFSM::setNewState(IClientState* State)
{
    delete CurrentState;
    CurrentState = State;
    CurrentState->setFSM(this);
}