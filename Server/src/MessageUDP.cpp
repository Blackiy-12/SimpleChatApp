#include "MessageUDP.hpp"
#include "String.hpp"
#include "Config.hpp"

std::vector<uint8_t> toVectorUDP(std::string String)
{
    std::vector<uint8_t> Result;

    for(auto& Letter: String)
    {
        Result.push_back(Letter);
    }

    return Result;
}

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

AuthMessageUDP::AuthMessageUDP(std::vector<uint8_t> Bytes)
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

        Content.Login.push_back(Bytes[i]);
    }

    LastIndex++;

    for (unsigned int i = LastIndex; i < Bytes.size() - 1; i++)
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
        LastIndex = i;
        if (Bytes[i] == 0x00)
        {
            break;
        }

        Content.Password.push_back(Bytes[i]);
    }
}

JoinMessageUDP::JoinMessageUDP(std::vector<uint8_t> Bytes)
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

        Content.Channel.push_back(Bytes[i]);
    }

    LastIndex++;

    for (unsigned int i = LastIndex; i < Bytes.size() - 1; i++)
    {
        LastIndex = i;
        if (Bytes[i] == 0x00)
        {
            break;
        }

        Content.Nickname.push_back(Bytes[i]);
    }
}

TextMessageUDP::TextMessageUDP(std::string Nickname, std::string MessageContent, uint16_t MessageID)
: ID(MessageID)
{
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);

    Content.Nickname = Nickname;
    for (const auto &Letter: Nickname )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    for(const auto &Letter : MessageContent)
    {
        Content.MessageContent.append(MessageContent);
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

ErrorMessageUDP::ErrorMessageUDP(std::string ErrorContent, std::string Nickname, uint16_t MessageID)
: ID(MessageID)
{ 
    Message = {};
    Message.push_back(MessageTypeValue);
    Message.push_back(ID & 0xFF);
    Message.push_back((ID >> 8) & 0xFF);

    SplittedString Str = ErrorContent;

    Content.Nickname = Nickname;
    for (const auto &Letter: Nickname )
    {
        Message.push_back(Letter);
    }

    Message.push_back(0x00);

    for(auto& Letter : ErrorContent)
    {
        Message.push_back(Letter);
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

    auto Vector = toVectorUDP(ReplyContent);
    Message.insert(Message.end(), Vector.begin(), Vector.end());
    
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