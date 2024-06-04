#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "Arguments.hpp"

enum class MessageType
{
    NONE,
    UNDEFINED,
    CONFIRM,
    AUTH,
    REPLY,
    JOIN,
    TEXT_MSG,
    ERROR,
    BYE
};

class IMessage
{
public:
    virtual ~IMessage() {};

    virtual std::vector<uint8_t> getMessageForSent() = 0;

    virtual MessageType getType() = 0;

    virtual uint16_t getID() = 0;
};

class NoneMessage: public IMessage
{
public:
    NoneMessage() {};

    ~NoneMessage() {};

    std::vector<uint8_t> getMessageForSent() { return {}; };

    MessageType getType() { return MessageType::NONE; };

    uint16_t getID() { return 0; };
};

class UndefinedMessage: public IMessage
{
public:
    UndefinedMessage() {};

    ~UndefinedMessage() {};

    std::vector<uint8_t> getMessageForSent() { return {}; };

    MessageType getType() { return MessageType::UNDEFINED; };

    uint16_t getID() { return 0; };
};

class IConfirmMessage: public IMessage
{
public:
    virtual ~IConfirmMessage() {};
};

class IAuthMessage: public IMessage
{
public:
    virtual ~IAuthMessage() {};

    virtual std::string getLogin() = 0;

    virtual std::string getNickname() = 0;

    virtual std::string getPassword() = 0;
};

class IJoinMessage: public IMessage
{
public:
    virtual ~IJoinMessage() {};

    virtual std::string getChannel() = 0;

    virtual std::string getNickname() = 0;
};

class ITextMessage: public IMessage
{
public:
    virtual ~ITextMessage() {};

    virtual std::string getNickname() = 0;

    virtual std::string getMessageContent() = 0;
};

class IErrorMessage: public IMessage
{
public:
    virtual ~IErrorMessage() {};

    virtual std::string getNickname() = 0;

    virtual std::string getMessageContent() = 0;
};

class IByeMessage: public IMessage
{
public:
    virtual ~IByeMessage() {};
};

class IReplyMessage: public IMessage
{
public:
    virtual ~IReplyMessage() {};

    virtual bool getResult() = 0;

    virtual uint16_t getReferencedID() = 0;

    virtual std::string getMessageContent() = 0;
};

class MessageCreator
{
public:
    static std::shared_ptr<IMessage> createMessageFromUser(std::string UserInput);

    static std::shared_ptr<IMessage> createMessageToConfirm(uint16_t MessageID);
    
    static std::shared_ptr<IMessage> createError(std::string ErrorContent);

    static std::shared_ptr<IMessage> createByeMessage();

    static std::shared_ptr<IMessage> translateMessageFromServer(std::vector<uint8_t> Bytes);

private:
    static std::shared_ptr<IMessage> createMessageFromUserUDP(std::string UserInput);
    static std::shared_ptr<IMessage> createMessageFromUserTCP(std::string UserInput);

    static std::shared_ptr<IMessage> translateMessageFromServerUDP(std::vector<uint8_t> Bytes);
    static std::shared_ptr<IMessage> translateMessageFromServerTCP(std::vector<uint8_t> Bytes);

    static std::shared_ptr<IMessage> createByeMessageUDP();
    static std::shared_ptr<IMessage> createByeMessageTCP();

    static std::shared_ptr<IMessage> createErrorUDP(std::string ErrorContent);
    static std::shared_ptr<IMessage> createErrorTCP(std::string ErrorContent);

};