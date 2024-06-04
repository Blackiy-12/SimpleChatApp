#pragma once

#include <string>
#include <vector>

class AuthenticationHandler
{
public:
    AuthenticationHandler();

    ~AuthenticationHandler();

    bool authUser(std::string Login, std::string Password);

    void removeUser(std::string Login);

private:
    std::vector<std::string> AuthUsers;
};