#pragma once 

#include <string>

class IO
{
public:
    static std::string getUserInput();

    static void printServerError(std::string DisplayName, std::string Error);

    static void printInternalError(std::string Error);

    static void printMessage(std::string DisplayName, std::string Message);

    static void printReply(bool Result, std::string Message);

    static void printHelp();
};