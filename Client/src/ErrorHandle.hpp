#pragma once

#include <string>

class App;

class ErrorHandle
{
public:
    static void throwError(std::string ErrorMessage);

    static void throwErrorAndExit(std::string ErrorMessage, int ExitCode);

    static void linkApp(App* AppPtr);

private:
    static App* AppPtr;
};