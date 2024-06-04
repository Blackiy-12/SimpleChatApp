#include "ClientState.hpp"

#include "ClientFSM.hpp"

IClientState::IClientState()
{
}

void IClientState::setFSM(ClientFSM* FSM)
{
    this->FSM = FSM;
}


bool StartState::isUserInputSuitable(std::shared_ptr<IMessage> Message)
{
    switch (Message->getType())
    {
    case MessageType::AUTH:
        return true;

    default:
        return false;
    }
}

void StartState::pushUserInput(std::shared_ptr<IMessage> Message)
{
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    case MessageType::AUTH:
        NewState = new AuthWaitState();
        FSM->setNewState(NewState);
        return;

    default:
        NewState = new ErrorState();
        FSM->setNewState(NewState);
        return;

    }
    
}

void StartState::pushServerInput(std::shared_ptr<IMessage> Message)
{       
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    default:
        NewState = new ErrorState();
        FSM->setNewState(NewState);
        return;
    }
}

bool AuthWaitState::isUserInputSuitable(std::shared_ptr<IMessage> Message)
{
    switch (Message->getType())
    {
    default:
        return false;
    }
}

void AuthWaitState::pushUserInput(std::shared_ptr<IMessage> Message)
{
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    default:
        NewState = new ErrorState();
        FSM->setNewState(NewState);
    }
    
}

void AuthWaitState::pushServerInput(std::shared_ptr<IMessage> Message)
{    
    IClientState* NewState = nullptr;
    IReplyMessage* ReplyMessage = nullptr;
    switch (Message->getType())
    {
    case MessageType::REPLY:
        ReplyMessage = dynamic_cast<IReplyMessage*>(Message.get());
        if (ReplyMessage->getResult() == true)
        {
            NewState = new OpenState();
        }
        else
        {
            NewState = new StartState();
        }

        FSM->setNewState(NewState);
        break;

    default:
        NewState = new ErrorState();
        FSM->setNewState(NewState);
        break;
    }
}

bool OpenState::isUserInputSuitable(std::shared_ptr<IMessage> Message)
{
    switch (Message->getType())
    {
    case MessageType::JOIN:
    case MessageType::TEXT_MSG:
    case MessageType::ERROR:
    case MessageType::BYE:
        return true;

    default:
        return false;
    }
}

void OpenState::pushUserInput(std::shared_ptr<IMessage> Message)
{
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    case MessageType::JOIN:
    case MessageType::TEXT_MSG:
        break;


    case MessageType::BYE:
        NewState = new EndState();
        FSM->setNewState(NewState);
        break;

    default:
        NewState = new ErrorState();
        FSM->setNewState(NewState);
        break;
    }
    
}

void OpenState::pushServerInput(std::shared_ptr<IMessage> Message)
{    
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    case MessageType::REPLY:
    case MessageType::TEXT_MSG:
        break;
        
    case MessageType::BYE:
        NewState = new EndState();
        FSM->setNewState(NewState);
        break;

    default:
        NewState = new ErrorState();
        FSM->setNewState(NewState);
        break;
    }
}

bool ErrorState::isUserInputSuitable(std::shared_ptr<IMessage> Message)
{
    switch (Message->getType())
    {
    default:
        return false;
    }
}

void ErrorState::pushUserInput(std::shared_ptr<IMessage> Message)
{
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    case MessageType::BYE:
        NewState = new EndState();
        FSM->setNewState(NewState);
        break;

    default:
        break;
    }
    
}

void ErrorState::pushServerInput(std::shared_ptr<IMessage> Message)
{    
    IClientState* NewState = nullptr;
    switch (Message->getType())
    {
    case MessageType::BYE:
        NewState = new EndState();
        FSM->setNewState(NewState);
        break;

    default:
        break;
    }
}

bool EndState::isUserInputSuitable(std::shared_ptr<IMessage> Message)
{
    switch (Message->getType())
    {
    default:
        return false;
    }
}

void EndState::pushUserInput(std::shared_ptr<IMessage> Message)
{
    switch (Message->getType())
    {
    default:
        break;
    }
    
}

void EndState::pushServerInput(std::shared_ptr<IMessage> Message)
{    
    switch (Message->getType())
    {
    default:
        break;
    }
}