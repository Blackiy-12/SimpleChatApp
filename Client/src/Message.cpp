#include "Message.hpp"
#include "MessageUDP.hpp"
#include "MessageTCP.hpp"
#include "String.hpp"
#include "Config.hpp"
#include "IO.hpp"
#include "MessageController.hpp"

const char* AuthInput = "/auth";

const char* JoinInput = "/join";

const char* Rename = "/rename";

const char* HelpInput = "/help";

std::shared_ptr<IMessage> MessageCreator::createMessageFromUser(std::string UserInput)
{
    if (Config::getConfig()->getProtocol() == Protocol::UDP)
        return createMessageFromUserUDP(UserInput);
    else 
        return createMessageFromUserTCP(UserInput);
}

std::shared_ptr<IMessage> MessageCreator::createMessageToConfirm(uint16_t MessageID)
{
    return std::make_shared<ConfirmMessageUDP>(MessageID);
}

 std::shared_ptr<IMessage> MessageCreator::createError(std::string ErrorContent)
 {
    if (Config::getConfig()->getProtocol() == Protocol::UDP)
    {
        return createErrorUDP(ErrorContent);
    }
    else
    {
        return createErrorTCP(ErrorContent);
    }
 }

std::shared_ptr<IMessage> MessageCreator::translateMessageFromServer(std::vector<uint8_t> Bytes)
{
    if (Config::getConfig()->getProtocol() == Protocol::UDP)
    {
        return MessageCreator::translateMessageFromServerUDP(Bytes);
    }
    else
    {
        return MessageCreator::translateMessageFromServerTCP(Bytes);
    }

}

std::shared_ptr<IMessage> MessageCreator::createMessageFromUserUDP(std::string UserInput)
{
    SplittedString Str(UserInput);
    auto MessageID = MessageController::getMessageController()->getUserProcessedMessage()->getNextMessageID();
    
    if (Str.getWord(0).compare(AuthInput) == 0)
        return std::make_shared<AuthMessageUDP>(UserInput, MessageID);

    else if (Str.getWord(0).compare(JoinInput) == 0)
        return std::make_shared<JoinMessageUDP>(UserInput, MessageID);

    else if (Str.getWord(0).compare(HelpInput) == 0)
    {
        IO::printHelp();
        return std::make_shared<NoneMessage>();
    }

    else if (Str.getWord(0).compare(Rename) == 0)
    {
        Config::getConfig()->setNickname(Str.getWord(1));
        return std::make_shared<NoneMessage>();
    }

    else
        return std::make_shared<TextMessageUDP>(UserInput, MessageID);
}

std::shared_ptr<IMessage> MessageCreator::createMessageFromUserTCP(std::string UserInput)
{
    SplittedString Str(UserInput);
    
    if (Str.getWord(0).compare(AuthInput) == 0)
        return std::make_shared<AuthMessageTCP>(UserInput);

    else if (Str.getWord(0).compare(JoinInput) == 0)
        return std::make_shared<JoinMessageTCP>(UserInput);

    else if (Str.getWord(0).compare(HelpInput) == 0)
    {
        IO::printHelp();
        return std::make_shared<NoneMessage>();
    }

    else if (Str.getWord(0).compare(Rename) == 0)
    {
        Config::getConfig()->setNickname(Str.getWord(1));
        return std::make_shared<NoneMessage>();
    }

    else
        return std::make_shared<TextMessageTCP>(UserInput);
}


std::shared_ptr<IMessage> MessageCreator::translateMessageFromServerUDP(std::vector<uint8_t> Bytes)
{
    uint8_t MessageCode = Bytes[0];

    switch (MessageCode)
    {
    case 0x00: //CONF
        return std::make_shared<ConfirmMessageUDP>(Bytes);
        break;
        
    case 0x01: //REPLY
        return std::make_shared<ReplyMessageUDP>(Bytes);
        break;

    case 0x04: //MSG
        return std::make_shared<TextMessageUDP>(Bytes);
        break;

    case 0xFE: //ERR
        return std::make_shared<ErrorMessageUDP>(Bytes);
        break;

    case 0xFF: //BYE
        return std::make_shared<ByeMessageUDP>(Bytes);
        break;

    default:
        break;
    }

    return std::make_shared<UndefinedMessage>();
}

std::shared_ptr<IMessage> MessageCreator::translateMessageFromServerTCP(std::vector<uint8_t> Bytes)
{
    uint8_t MessageCode = Bytes[0];

    switch (MessageCode)
    {        
    case 'R': //REPLY
        return std::make_shared<ReplyMessageTCP>(Bytes);
        break;

    case 'M': //MSG
        return std::make_shared<TextMessageTCP>(Bytes);
        break;

    case 'E': //ERR
        return std::make_shared<ErrorMessageTCP>(Bytes);
        break;

    case 'B': //BYE
        return std::make_shared<ByeMessageTCP>(Bytes);
        break;

    default:
        break;
    }

    return std::make_shared<UndefinedMessage>();
}


std::shared_ptr<IMessage> MessageCreator::createByeMessage()
{
    if (Config::getConfig()->getProtocol() == Protocol::UDP)
    {
        return createByeMessageUDP();
    }
    else 
    {
        return createByeMessageTCP();
    }
}

std::shared_ptr<IMessage> MessageCreator::createByeMessageUDP()
{
    auto MessageID = MessageController::getMessageController()->getUserProcessedMessage()->getNextMessageID();
    return std::make_shared<ByeMessageUDP>(MessageID);
}

std::shared_ptr<IMessage> MessageCreator::createByeMessageTCP()
{
    return std::make_shared<ByeMessageTCP>();
}

std::shared_ptr<IMessage> MessageCreator::createErrorUDP(std::string ErrorContent)
{
    auto MessageID = MessageController::getMessageController()->getUserProcessedMessage()->getNextMessageID();
    return std::make_shared<ErrorMessageUDP>(ErrorContent, MessageID);
}

std::shared_ptr<IMessage> MessageCreator::createErrorTCP(std::string ErrorContent)
{
    return std::make_shared<ErrorMessageTCP>(ErrorContent);
}