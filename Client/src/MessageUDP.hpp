#pragma once
#include "Message.hpp"

class ConfirmMessageUDP : public IConfirmMessage
{
public:
    ConfirmMessageUDP(uint16_t MessageID);
    ConfirmMessageUDP(std::vector<uint8_t> Bytes);
    ~ConfirmMessageUDP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::CONFIRM; };

    uint16_t getID() override { return ID; };

private:
    const uint8_t MessageTypeValue = 0x00;

    uint16_t ID;

    std::vector<uint8_t> Message;
};

class AuthMessageUDP : public IAuthMessage
{
public:
    AuthMessageUDP(std::string UserInput, uint16_t MessageID);
    ~AuthMessageUDP() {};

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

    uint16_t ID;

    std::vector<uint8_t> Message;
};

class JoinMessageUDP: public IJoinMessage
{
public:
    JoinMessageUDP(std::string UserInput, uint16_t MessageID);
    ~JoinMessageUDP() {};

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

    uint16_t ID;
};

class TextMessageUDP: public ITextMessage
{
public:
    TextMessageUDP(std::string UserInput, uint16_t MessageID);
    TextMessageUDP(std::vector<uint8_t> Bytes);
    ~TextMessageUDP() {};

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

    uint16_t ID;
};

class ErrorMessageUDP: public IErrorMessage
{
public:
    ErrorMessageUDP(std::string ErrorContent, uint16_t MessageID);
    ErrorMessageUDP(std::vector<uint8_t> Bytes);
    ~ErrorMessageUDP() {};

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

    uint16_t ID;
};

class ByeMessageUDP : public IByeMessage
{
public:
    ByeMessageUDP(uint16_t MessageID);
    ByeMessageUDP(std::vector<uint8_t> Bytes);
    ~ByeMessageUDP() {};

    std::vector<uint8_t> getMessageForSent() override { return Message; };
    
    MessageType getType() override { return MessageType::BYE; };

    uint16_t getID() override { return ID; };

private:
    const uint8_t MessageTypeValue = 0xFF;

    uint16_t ID;

    std::vector<uint8_t> Message;
};

class ReplyMessageUDP: public IReplyMessage
{
public:
    ReplyMessageUDP(std::string ReplyContent, uint16_t MessageID, uint8_t Result, uint16_t RefMessageID);
    ReplyMessageUDP(std::vector<uint8_t> Bytes);
    ~ReplyMessageUDP() {};

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

    uint16_t ID;
};