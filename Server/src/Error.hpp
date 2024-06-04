#pragma once

#include <string>

enum class ErrorCode
{
    ERROR_WITH_CONNECTION = 11,
    ERROR_WITH_EPOLL = 12,
    OTHERS = 99
};

class App;

class ErrorHandler
{
public:
    static void throwError(ErrorCode Code, std::string ErrorInfo = "Error raise");

    static void linkApp(App* AppPtr);

private:
    static App* AppPtr;
};