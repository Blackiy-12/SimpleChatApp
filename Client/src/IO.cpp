#include "IO.hpp"

#include <iostream>

std::string IO::getUserInput()
{
    std::string Result;
    getline(std::cin, Result);
    return Result;
}

void IO::printServerError(std::string DisplayName, std::string Error)
{
    std::cerr << "ERR FROM " << DisplayName << ": " << Error << "\n";
}

void IO::printInternalError(std::string Error)
{
    std::cerr << "ERR: " << Error << "\n";
}

void IO::printMessage(std::string DisplayName, std::string Message)
{
    std::cout << DisplayName << ": " << Message << "\n";
}

void IO::printReply(bool Result, std::string Message)
{
    if (Result == true)
    {
        std::cout << "Success: " <<  Message << "\n";
    }
    else 
    {
        std::cout << "Failure: " <<  Message << "\n";
    }
}


void IO::printHelp()
{
    std::cout << "Authorize:\n";
    std::cout << "/auth {Username} {Secret} {DisplayName}\n";

    std::cout << "Rename:\n";
    std::cout << "/rename {DisplayName}\n";
    
    std::cout << "Join:\n";
    std::cout << "/join {ChannelID}\n";

    std::cout << "Text message: Just print it\n";
}