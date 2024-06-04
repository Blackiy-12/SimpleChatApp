#include "AuthenticationHandler.hpp"


AuthenticationHandler::AuthenticationHandler()
{
    AuthUsers = {};
}

AuthenticationHandler::~AuthenticationHandler()
{

}

bool AuthenticationHandler::authUser(std::string Login, std::string Password)
{
    for (auto &User : AuthUsers)
    {
        if(User == Login)
            return false;

    }

    AuthUsers.push_back(Login);
    return true;
}

void AuthenticationHandler::removeUser(std::string Login)
{
    for (int i = 0; i < AuthUsers.size(); i++)
    {
        if(AuthUsers.at(i) == Login)
        {
            AuthUsers.erase(AuthUsers.begin() + i);
            return;
        }

    }
}