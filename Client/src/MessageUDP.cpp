#include "MessageUDP.hpp"
#include "String.hpp"
#include "Config.hpp"

ConfirmMessageUDP::ConfirmMessageUDP(uint16_t MessageID)
: ID(MessageID)
{
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);
}


ConfirmMessageUDP::ConfirmMessageUDP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    ID = Bytes[1];
    ID |= Bytes[2] << 8;
}

AuthMessageUDP::AuthMessageUDP(std::string UserInput, uint16_t MessageID)
: ID(MessageID)
{
    SplittedString Str(UserInput);
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);


    auto User = Str.getWord(1);
    Content.Login = User;
    for (const auto &Letter: User )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    auto DisplayName = Str.getWord(3);
    Content.Nickname = DisplayName;
    Config::getConfig()->setNickname(DisplayName);
    for (const auto &Letter: DisplayName )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    auto Password = Str.getWord(2);
    Content.Password = Password;
    for (const auto &Letter: Password )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

}

JoinMessageUDP::JoinMessageUDP(std::string UserInput, uint16_t MessageID)
: ID(MessageID)
{
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);

    SplittedString Str = UserInput;

    auto Channel = Str.getWord(1);
    Content.Channel = Channel;
    for (const auto &Letter: Channel )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    auto Nick = Config::getConfig()->getNickname();
    Content.Nickname = Nick;
    for (const auto &Letter: Nick )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);
}

TextMessageUDP::TextMessageUDP(std::string UserInput, uint16_t MessageID)
: ID(MessageID)
{
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);

    auto Nick = Config::getConfig()->getNickname();
    Content.Nickname = Nick;
    for (const auto &Letter: Nick )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    for (const auto &Letter: UserInput )
    {
        Message.push_back(Letter);
    }
    
    Message.push_back(0x00);
}


TextMessageUDP::TextMessageUDP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    ID = Bytes[1];
    ID |= Bytes[2] << 8;

    unsigned int LastIndex = 3;
    for (unsigned int i = 3; i < Bytes.size() - 1; i++)
    {
        LastIndex = i;
        if (Bytes[i] == 0x00)
        {
            break;
        }

        Content.Nickname.push_back(Bytes[i]);
    }

    LastIndex++;
    
    for (unsigned int i = LastIndex; i < Bytes.size() - 1; i++)
    {
        Content.MessageContent.push_back(Bytes[i]);
    }

}

ErrorMessageUDP::ErrorMessageUDP(std::string ErrorContent, uint16_t MessageID)
: ID(MessageID)
{ 
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);

    SplittedString Str = ErrorContent;

    auto Nick = Config::getConfig()->getNickname();
    Content.Nickname = Nick;
    for (const auto &Letter: Nick )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    for(int i = 0; i < Str.getWordsCount(); i++)
    {
        Content.MessageContent.append(Str.getWord(i));
        for (const auto &Letter: Str.getWord(i) )
        {
            Message.push_back(Letter);
        }
    }
    
    Message.push_back(0x00);
}

ErrorMessageUDP::ErrorMessageUDP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    ID = Bytes[1];
    ID |= Bytes[2] << 8;

    unsigned int LastIndex = 3;
    for (unsigned int i = 3; i < Bytes.size() - 1; i++)
    {
        LastIndex = i;
        if (Bytes[i] == 0x00)
        {
            break;
        }

        Content.Nickname.push_back(Bytes[i]);
    }

    LastIndex++;
    
    for (unsigned int i = LastIndex; i < Bytes.size() - 1; i++)
    {
        Content.MessageContent.push_back(Bytes[i]);
    }

}

ByeMessageUDP::ByeMessageUDP(uint16_t MessageID)
: ID(MessageID)
{
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);
}

ByeMessageUDP::ByeMessageUDP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    ID = Bytes[1];
    ID |= Bytes[2] << 8;
}

ReplyMessageUDP::ReplyMessageUDP(std::string ReplyContent, uint16_t MessageID, uint8_t Result, uint16_t RefMessageID)
: ID(MessageID)
{
    Content.Result = Result;

    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);
    Message.push_back(Result);
    Message.push_back(RefMessageID);

    SplittedString Str = ReplyContent;

    Message.push_back(0x00);

    for(int i = 0; i < Str.getWordsCount(); i++)
    {
        Content.MessageContent.append(Str.getWord(i));
        for (const auto &Letter: Str.getWord(i) )
        {
            Message.push_back(Letter);
        }
    }
    
    Message.push_back(0x00);
}


ReplyMessageUDP::ReplyMessageUDP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    ID = Bytes[1];
    ID |= Bytes[2] << 8;

    
    Content.Result = Bytes[3];

    
    Content.RefID = Bytes[4];
    Content.RefID |= Bytes[5] << 8;

    for (unsigned int i = 6; i < Bytes.size() - 1; i++)
    {
        Content.MessageContent.push_back(Bytes[i]);
    }
}