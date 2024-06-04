#include "MessageController.hpp"

MessageController* MessageController::MessageControllerPtr = nullptr;

MessageController* MessageController::getMessageController()
{
    if (MessageControllerPtr == nullptr)
        MessageControllerPtr = new MessageController();

    return MessageControllerPtr;
}

MessageController::MessageController()
{

    MessageHolderPtr = std::make_unique<MessageHolder>();

    UserProcessedMessagePtr = std::make_unique<UserProcessedMessage>();

    ServerProcessedMessagePtr = std::make_unique<ServerProcessedMessage>();

}

MessageController::~MessageController()
{

}