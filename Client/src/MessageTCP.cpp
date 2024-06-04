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

AuthMessageTCP::AuthMessageTCP(std::string UserInput)
{
    SplittedString Input(UserInput);

    auto Vector = toVector("AUTH ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Login
    Content.Login = Input.getWord(1);
    for (auto& Letter: Content.Login)
    {
        Message.push_back(Letter);
    }

    Vector = toVector(" AS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Input.getWord(3);
    Config::getConfig()->setNickname(Content.Nickname);
    for (auto& Letter: Content.Nickname)
    {
        Message.push_back(Letter);
    }

    Vector = toVector(" USING ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Password
    Content.Password = Input.getWord(2);
    for (auto& Letter: Content.Password)
    {
        Message.push_back(Letter);
    }

    Vector = toVector("\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}

JoinMessageTCP::JoinMessageTCP(std::string UserInput)
{
    SplittedString Input(UserInput);

    auto Vector = toVector("JOIN ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Channel
    Content.Channel = Input.getWord(1);
    for (auto& Letter: Content.Channel)
    {
        Message.push_back(Letter);
    }

    Vector = toVector(" AS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Config::getConfig()->getNickname();
    for (auto& Letter: Content.Nickname)
    {
        Message.push_back(Letter);
    }

    Vector = toVector("\r\n");
    Message.insert(Message.end(), Vector.begin(), Vector.end());
}


TextMessageTCP::TextMessageTCP(std::string UserInput)
{

    auto Vector = toVector("MSG FROM ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Config::getConfig()->getNickname();
    for (auto& Letter: Content.Nickname)
    {
        Message.push_back(Letter);
    }

    Vector = toVector(" IS ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    Content.MessageContent.append(UserInput);
    
    Vector = toVector(UserInput);
    Message.insert(Message.end(), Vector.begin(), Vector.end());

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

ErrorMessageTCP::ErrorMessageTCP(std::string ErrorContent)
{
    auto Vector = toVector("ERR FROM ");
    Message.insert(Message.end(), Vector.begin(), Vector.end());

    //Nickname
    Content.Nickname = Config::getConfig()->getNickname();
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