#pragma once

#include <memory>
#include "MessageHolder.hpp"
#include "UserProcessedMessage.hpp"
#include "ServerProcessedMessage.hpp"

class MessageController
{
public:
    static MessageController* getMessageController();

    MessageHolder* getMessageHolder() { return MessageHolderPtr.get(); }

    UserProcessedMessage* getUserProcessedMessage() { return UserProcessedMessagePtr.get(); }

    ServerProcessedMessage* getServerProcessedMessage() { return ServerProcessedMessagePtr.get(); }

private:
    MessageController();
    
    ~MessageController();

private:
    static MessageController* MessageControllerPtr;

    std::unique_ptr<MessageHolder> MessageHolderPtr;

    std::unique_ptr<UserProcessedMessage> UserProcessedMessagePtr;

    std::unique_ptr<ServerProcessedMessage> ServerProcessedMessagePtr;

};