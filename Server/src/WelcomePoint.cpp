#include "WelcomePoint.hpp"

#include "Error.hpp"

#include "ChannelHolder.hpp"

WelcomePoint::WelcomePoint()
{
    Connection = nullptr;
    AuthenticationHandlerPtr = nullptr;
    PointProtocol = Protocol::NONE;
    MessageBuffer = nullptr;
}
   
WelcomePoint::~WelcomePoint()
{

}

void WelcomePoint::createConnection(Protocol Protocol, std::string ServerAddress, uint16_t ServerPort)
{
    PointProtocol = Protocol;

    MessageBuffer = MessageBufferCreator::createMessageBuffer(PointProtocol);

    Connection = ConnectionCreator::createConnection(Protocol, ServerAddress, ServerPort);
    Connection->createSocket();
    Connection->bindSocket();
    Connection->linkMessageBuffer(MessageBuffer.get());
}

void WelcomePoint::linkToAuthenticationHandler(AuthenticationHandler* Handler)
{
    AuthenticationHandlerPtr = Handler;
}

std::shared_ptr<User> WelcomePoint::processNewUser(ChannelHolder* Channels)
{
    if (PointProtocol == Protocol::UDP)
    {
        Connection->receiveMessage();

        auto Message = MessageBuffer->getMessage();

        if (Message->getType() != MessageType::AUTH)
        {
            auto ErrMSG = MessageCreator::createError(PointProtocol, "Try to communicate with welcome port");
            Connection->sendMessage(ErrMSG);
            return nullptr;
        }

        auto NewUser = std::make_shared<User>(PointProtocol, Channels, AuthenticationHandlerPtr);

        NewUser->createConnection(Connection->getLastAddress());

        auto AUTH_MSG = dynamic_cast<IAuthMessage*>(Message.get());

        NewUser->processAuth(AUTH_MSG);
        
        return NewUser;
    }
    else
    {
        auto NewUser = std::make_shared<User>(PointProtocol, Channels, AuthenticationHandlerPtr);

        NewUser->acceptConnection(Connection->getSocket(), Connection->getLastAddress());

        return NewUser;
    }

}

int WelcomePoint::getSocket()
{
    if (Connection == nullptr)
    {
        ErrorHandler::throwError(ErrorCode::ERROR_WITH_CONNECTION, "Try to get socket from zero connection");
    }

    return Connection->getSocket();
}