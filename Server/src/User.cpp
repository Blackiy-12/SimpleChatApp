#include "User.hpp"
 
#include "Message.hpp"
#include "AuthenticationHandler.hpp"
#include "IO.hpp"
#include "Converter.hpp"

User::User(Protocol Protocol, ChannelHolder* Holder, AuthenticationHandler* Handler)
{
    UserProtocol = Protocol;
    if(UserProtocol == Protocol::UDP)
    {
        Data = std::make_unique<UserData>();
    }
    Channels = Holder;
    CurrentChannel = nullptr;
    Login = "";
    LastNick = "";
    State = UserState::AUTH;
    AuthHandler = Handler;

    MessageBuffer = MessageBufferCreator::createMessageBuffer(UserProtocol);
}

User::~User()
{

}

void User::acceptConnection(int WelcomeSocket, struct addrinfo* Address)
{
    Connection = ConnectionCreator::createConnection(UserProtocol, Address);
    Connection->acceptConnection(WelcomeSocket);
    Connection->linkMessageBuffer(MessageBuffer.get());
}

void User::createConnection(struct addrinfo* Address)
{
    Connection = ConnectionCreator::createConnection(UserProtocol, Address);
    Connection->createSocket();
    Connection->linkMessageBuffer(MessageBuffer.get());
}

int User::getUserSocket()
{
    return Connection->getSocket();
}

void User::sendMessageToUser(std::string Nickname, std::string Content)
{
    std::shared_ptr<IMessage> Message = nullptr;

    if (UserProtocol == Protocol::UDP)
    {
        Message = MessageCreator::createMessage(UserProtocol, Nickname, Content, Data->getMessageID());
    }
    else
    {
        Message = MessageCreator::createMessage(UserProtocol, Nickname, Content);
    }

    bool Result = Connection->sendMessage(Message);

    if (Result == false)
    {
        endWithError("Error with sending data");
    }
}

void User::receiveMessage()
{
    Connection->receiveMessage();

    while (MessageBuffer->hasMessage())
    {
        auto Message = MessageBuffer->getMessage();

        if (UserProtocol == Protocol::TCP)
        {
            auto IP = Connection->getIP();
            auto Port = Connection->getPort();
            auto Type = ConverterToString::convertMessageType(Message->getType());
            IO::printReceiveMessage(IP, Port, Type);
        }
        else
        {
            if (Data->isMessageProcessed(Message->getID()) == true && Message->getType() != MessageType::UNDEFINED)
            {
                continue;
            }
        }

        std::shared_ptr<IMessage> Reply = nullptr;

        switch (Message->getType())
        {
        case MessageType::NONE:
            break;

        case MessageType::UNDEFINED:
        case MessageType::REPLY:
            endWithError("Received unexpected message");
            break;

        case MessageType::CONFIRM:
            break;

        case MessageType::AUTH:
            if (State != UserState::AUTH)
            {
                endWithError("Received unexpected message");
            }
            else
            {  
                processAuth(dynamic_cast<IAuthMessage*>(Message.get()));
            }
            break;


        case MessageType::JOIN:
            if (State != UserState::OPEN)
            {
                sendReply(false, "You can't join channels", Message->getID());
            }
            else
            {
                joinChannel(dynamic_cast<IJoinMessage*>(Message.get()));
            }
            break;

        case MessageType::TEXT_MSG:
            if (State != UserState::OPEN)
            {
                sendReply(false, "You can't text", Message->getID());
            }
            else
            {
                processMessage(dynamic_cast<ITextMessage*>(Message.get()));
            }
            break;

        case MessageType::ERROR:
            sendBye();
            State = UserState::BYE;
            break;

        case MessageType::BYE:
            State = UserState::BYE;
            AuthHandler->removeUser(Login);
            if (CurrentChannel != nullptr)
            {
                CurrentChannel->leave(LastNick, this);
                CurrentChannel = nullptr;
            }
            break;

        default:
            break;
        }
    }
}

void User::processAuth(IAuthMessage* Message)
{
    bool Result = AuthHandler->authUser(Message->getLogin(), Message->getPassword());

    if (Result == true)
    {
        Login = Message->getLogin();
        LastNick = Message->getNickname();

        auto DefaultChannel = Channels->getChannel();

        DefaultChannel->join(LastNick, this);

        CurrentChannel = DefaultChannel;

        std::shared_ptr<IMessage> Reply = nullptr;

        State = UserState::OPEN;

        sendReply(true, "Success auth, you join Default channel", Message->getID());

    }
    else
    {
        std::shared_ptr<IMessage> Reply = nullptr;

        sendReply(false, "Can't auth you", Message->getID());
    }

}

void User::sendBye()
{   
    
    if (State == UserState::BYE)
    {
        return;
    }
    
    AuthHandler->removeUser(Login);
    
    if (CurrentChannel != nullptr)
    {
        CurrentChannel->leave(LastNick, this);
        CurrentChannel = nullptr;
    }

    std::shared_ptr<IMessage> Message = nullptr;

    if (UserProtocol == Protocol::UDP)
    {
        Message = MessageCreator::createByeMessage(UserProtocol, Data->getMessageID());
    }
    else
    {
        Message = MessageCreator::createByeMessage(UserProtocol);
    }

    if (CurrentChannel != nullptr)
    {
        CurrentChannel->leave(LastNick, this);
    }

    Connection->sendMessage(Message);
    State = UserState::BYE;
}

void User::endWithError(std::string ErrorContent)
{
    if (CurrentChannel != nullptr)
    {
        CurrentChannel->leave(LastNick, this);
        CurrentChannel = nullptr;
    }

    std::shared_ptr<IMessage> Message = nullptr;

    if (UserProtocol == Protocol::UDP)
    {
        Message = MessageCreator::createError(UserProtocol, ErrorContent, Data->getMessageID());
    }
    else
    {
        Message = MessageCreator::createError(UserProtocol, ErrorContent);
    }

    Connection->sendMessage(Message);
    State = UserState::ERR;

    sendBye();
}

std::string User::getLastNick()
{
    return  LastNick;
}

UserState User::getCurrentState()
{
    return State;
}


void User::sendReply(bool Result, std::string ReplyContent, uint16_t RefID)
{
    std::shared_ptr<IMessage> Reply = nullptr;

    if (UserProtocol == Protocol::UDP)
    {
        Reply = MessageCreator::createReply(UserProtocol, Result, ReplyContent, Data->getMessageID(), RefID);
    }
    else
    {
        Reply = MessageCreator::createReply(UserProtocol, Result, ReplyContent);
    }

    bool ResultOfSending = Connection->sendMessage(Reply);

    if (ResultOfSending == false)
    {
        endWithError("Can't send message");
    }
}

void User::joinChannel(IJoinMessage* Message)
{
    LastNick = Message->getNickname();

    auto newChannel = Channels->getChannel(Message->getChannel());
    CurrentChannel->leave(LastNick, this);

    newChannel->join(LastNick, this);

    CurrentChannel = newChannel;

    sendReply(true, "You join channel", Message->getID());
}


void User::processMessage(ITextMessage* Message)
{
    LastNick = Message->getNickname();

    CurrentChannel->sendMessageAllUsers(LastNick, Message->getMessageContent(), this);
}