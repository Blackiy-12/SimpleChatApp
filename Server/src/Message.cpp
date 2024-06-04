#include "Message.hpp"
#include "MessageUDP.hpp"
#include "MessageTCP.hpp"
#include "String.hpp"
#include "Config.hpp"
#include "IO.hpp"


std::shared_ptr<IMessage> MessageCreator::createMessage(Protocol Protocol, std::string Nickname, std::string MessageContent, uint16_t MessageID)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_shared<TextMessageUDP>(Nickname, MessageContent, MessageID);
    }
    else
    {
        return std::make_shared<TextMessageTCP>(Nickname, MessageContent);
    }
}

std::shared_ptr<IMessage> MessageCreator::createReply(Protocol Protocol, bool Result, std::string MessageContent, uint16_t MessageID, uint16_t RefMessage)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_shared<ReplyMessageUDP>(MessageContent, MessageID, Result, RefMessage);
    }
    else
    {
        return std::make_shared<ReplyMessageTCP>(MessageContent, Result);
    }
}

std::shared_ptr<IMessage> MessageCreator::createMessageToConfirm(uint16_t MessageID)
{
    return std::make_shared<ConfirmMessageUDP>(MessageID);
}
    
std::shared_ptr<IMessage> MessageCreator::createError(Protocol Protocol, std::string ErrorContent, uint16_t MessageID)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_shared<ErrorMessageUDP>(ErrorContent, "Server", MessageID);
    }
    else
    {
        return std::make_shared<ErrorMessageTCP>(ErrorContent, "Server");
    }
}

std::shared_ptr<IMessage> MessageCreator::createByeMessage(Protocol Protocol, uint16_t MessageID)
{
    if (Protocol == Protocol::UDP)
    {
        return std::make_shared<ByeMessageUDP>(MessageID);
    }
    else
    {
        return std::make_shared<ByeMessageTCP>();
    }
}

std::shared_ptr<IMessage> MessageCreator::translateMessage(Protocol Protocol, std::vector<uint8_t> Bytes)
{
    if (Protocol == Protocol::UDP)
    {
        return translateMessageUDP(Bytes);
    }
    else
    {
        return translateMessageTCP(Bytes);
    }
}


std::shared_ptr<IMessage> MessageCreator::translateMessageUDP(std::vector<uint8_t> Bytes)
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

    case 0x02://AUTH
        return std::make_shared<AuthMessageUDP>(Bytes);
        break;

    case 0x03://JOIN
        return std::make_shared<JoinMessageUDP>(Bytes);
        break;
        
    default:
        break;
    }

    return std::make_shared<UndefinedMessage>();
}

std::shared_ptr<IMessage> MessageCreator::translateMessageTCP(std::vector<uint8_t> Bytes)
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

    case 'A': //AUTH
        return std::make_shared<AuthMessageTCP>(Bytes);
        break;

    case 'J'://JOIN
        return std::make_shared<JoinMessageTCP>(Bytes);
        break;

    default:
        break;
    }

    return std::make_shared<UndefinedMessage>();
}