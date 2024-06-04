#include "MessageTCP.hpp"
#include "String.hpp"
#include "Config.hpp"

std::vector<uint8_t> toVector(std::string String)
{
    std::vector<uint8_t> Result;

    for(auto& Letter: String)
    {
        Result.push_back(Letter);
    }

    return Result;
}

AuthMessageTCP::AuthMessageTCP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    unsigned int i = 5;
    while(i < Bytes.size())
    {
        if(Bytes[i] == ' ')
            break;

        Content.Login.push_back(Bytes[i]);
        i++;
    }
    i += 4;

    while (i < Bytes.size())
    {
        if(Bytes[i] == ' ')
            break;

        Content.Nickname.push_back(Bytes[i]);
        i++;
    }

    i+=7;

    while (i < Bytes.size())
    {
        if(Bytes[i] == '\r')
            break;

        Content.Password.push_back(Bytes[i]);
        i++;
    }
}

JoinMessageTCP::JoinMessageTCP(std::string Channel, std::string Nickname)
{
    auto Vector = toVector("JOIN ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Channel
    Content.Channel = Channel;
    for (auto& Letter: Content.Channel)
    {
        Message.push_back(Letter);
    }

    Vector = toVector(" AS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Nickname;
    for (auto& Letter: Content.Nickname)
    {
        Message.push_back(Letter);
    }

    Vector = toVector("\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}

JoinMessageTCP::JoinMessageTCP(std::vector<uint8_t> Bytes)
{
    
    Message = Bytes;

    unsigned int i = 5;
    while(i < Bytes.size())
    {
        if(Bytes[i] == ' ')
            break;

        Content.Channel.push_back(Bytes[i]);
        i++;
    }
    i += 4;

    while (i < Bytes.size())
    {
        if(Bytes[i] == '\r')
            break;

        Content.Nickname.push_back(Bytes[i]);
        i++;
    }
}

TextMessageTCP::TextMessageTCP(std::string Nickname, std::string MessageContent)
{
    auto Vector = toVector("MSG FROM ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Nickname;
    for (auto& Letter: Content.Nickname)
    {
        Message.push_back(Letter);
    }

    Vector = toVector(" IS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    for (auto Letter: MessageContent)
    {
        Content.MessageContent.push_back(Letter);
        Message.push_back(Letter);
    }


    Vector = toVector("\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}

TextMessageTCP::TextMessageTCP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    unsigned int i = 9;
    while(i < Bytes.size())
    {
        if(Bytes[i] == ' ')
            break;

        Content.Nickname.push_back(Bytes[i]);
        i++;
    }
    i += 4;

    while (i < Bytes.size())
    {
        if(Bytes[i] == '\r')
            break;

        Content.MessageContent.push_back(Bytes[i]);
        i++;
    }
    
}

ErrorMessageTCP::ErrorMessageTCP(std::string ErrorContent, std::string Nickname)
{
    auto Vector = toVector("ERR FROM ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Nickname;
    Vector = toVector(Content.Nickname);
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    Vector = toVector(" IS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    for (auto& Letter : ErrorContent)
    {
        Content.MessageContent.push_back(Letter);
        Message.push_back(Letter);
    }

    Vector = toVector("\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}

ErrorMessageTCP::ErrorMessageTCP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;

    unsigned int i = 9;
    while(i < Bytes.size())
    {
        if(Bytes[i] == ' ')
            break;

        Content.Nickname.push_back(Bytes[i]);
        i++;
    }
    i += 4;

    while (i < Bytes.size())
    {
        if(Bytes[i] == '\r')
            break;

        Content.MessageContent.push_back(Bytes[i]);
        i++;
    }
}

ByeMessageTCP::ByeMessageTCP()
{
    
    auto Vector = toVector("BYE\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}

ByeMessageTCP::ByeMessageTCP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;
}


ReplyMessageTCP::ReplyMessageTCP(std::string ReplyContent, uint8_t Result)
{
    auto Vector = toVector("REPLY ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    Content.RefID = 0;
    Content.Result = Result;
    Content.MessageContent = ReplyContent;

    if (Content.Result == true)
    {
        Vector = toVector("OK");
        Message.insert(Message.end(), Vector.begin(), Vector.end());
    }
    else 
    {
        Vector = toVector("NOK");
        Message.insert(Message.end(), Vector.begin(), Vector.end());
    }

    Vector = toVector(" IS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    for(auto& Letter: ReplyContent)
    {
        Message.push_back(Letter);
    }

    Vector = toVector("\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}

ReplyMessageTCP::ReplyMessageTCP(std::vector<uint8_t> Bytes)
{
    Message = Bytes;
    Content.RefID = 0;
    unsigned int i = 0;

    if (Bytes[6] == 'N')
    {
        Content.Result = false;
        i++;
    }
    else 
    {
        Content.Result = true;
    }
    i+= 12;

    while (i < Bytes.size())
    {
        if(Bytes[i] == '\r')
            break;

        Content.MessageContent.push_back(Bytes[i]);
        i++;
    }


}