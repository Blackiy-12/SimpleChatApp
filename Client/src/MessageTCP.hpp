#pragma once
#include "Message.hpp"

class AuthMessageTCP : public IAuthMessage
{
public:
    AuthMessageTCP(std::string UserInput);
    ~AuthMessageTCP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::AUTH; };

    std::string getLogin() override { return Content.Login; };

    std::string getNickname() override { return Content.Nickname; };

    std::string getPassword() override { return Content.Password; };

    uint16_t getID() override { return ID; };
private:

    struct MessageStructure
    {
        std::string Nickname = "";
        std::string Login = "";
        std::string Password = "";
    };

    MessageStructure Content;

    const uint8_t MessageTypeValue = 0x02;

    uint16_t ID = 0;

    std::vector<uint8_t> Message;
};

class JoinMessageTCP: public IJoinMessage
{
public:
    JoinMessageTCP(std::string UserInput);
    ~JoinMessageTCP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::JOIN; };

    uint16_t getID() override { return ID; };

    std::string getChannel() override { return Content.Channel; };

    std::string getNickname() override { return Content.Nickname; };

private:

    std::vector<uint8_t> Message;

    struct MessageStructure
    {
        std::string Nickname = "";
        std::string Channel = "";
    };

    MessageStructure Content;

    const uint8_t MessageTypeValue = 0x03;

    uint16_t ID = 0;
};

class TextMessageTCP: public ITextMessage
{
public:
    TextMessageTCP(std::string UserInput);
    TextMessageTCP(std::vector<uint8_t> Bytes);
    ~TextMessageTCP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::TEXT_MSG; };

    uint16_t getID() override { return ID; };

    std::string getNickname() override { return Content.Nickname; };
    
    std::string getMessageContent() override { return Content.MessageContent; };

private:

    std::vector<uint8_t> Message;

    struct MessageStructure
    {
        std::string Nickname = "";
        std::string MessageContent = "";
    };

    MessageStructure Content;

    const uint8_t MessageTypeValue = 0x04;

    uint16_t ID = 0;
};

class ErrorMessageTCP: public IErrorMessage
{
public:
    ErrorMessageTCP(std::string ErrorContent);
    ErrorMessageTCP(std::vector<uint8_t> Bytes);
    ~ErrorMessageTCP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::ERROR; };

    uint16_t getID() override { return ID; };

    std::string getNickname() override { return Content.Nickname; };
    
    std::string getMessageContent() override { return Content.MessageContent; };

private:

    std::vector<uint8_t> Message;

    struct MessageStructure
    {
        std::string Nickname = "";
        std::string MessageContent = "";
    };

    MessageStructure Content;

    const uint8_t MessageTypeValue = 0xFE;

    uint16_t ID = 0;
};

class ByeMessageTCP : public IByeMessage
{
public:
    ByeMessageTCP();
    ByeMessageTCP(std::vector<uint8_t> Bytes);
    ~ByeMessageTCP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::BYE; };

    uint16_t getID() override { return ID; };

private:
    const uint8_t MessageTypeValue = 0xFF;

    uint16_t ID = 0;

    std::vector<uint8_t> Message;
};

class ReplyMessageTCP: public IReplyMessage
{
public:
    ReplyMessageTCP(std::string ReplyContent, uint8_t Result);
    ReplyMessageTCP(std::vector<uint8_t> Bytes);
    ~ReplyMessageTCP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::REPLY; };

    uint16_t getID() override { return ID; };

    bool getResult() override { return (Content.Result == 0x01) ? true : false; };

    uint16_t getReferencedID() override { return Content.RefID; };
    
    std::string getMessageContent() override { return Content.MessageContent; };

private:

    std::vector<uint8_t> Message;

    struct MessageStructure
    {
        std::string MessageContent = "";
        uint8_t Result = 0;
        uint16_t RefID = 0;
    };

    MessageStructure Content;

    const uint8_t MessageTypeValue = 0x01;

    uint16_t ID = 0;
};