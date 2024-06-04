#include "Error.hpp"

#include "App.hpp"

App* ErrorHandler::AppPtr = nullptr;

void ErrorHandler::linkApp(App* AppPtr)
{
    if (ErrorHandler::AppPtr == nullptr)
    {
        ErrorHandler::AppPtr = AppPtr;
    }
}


void ErrorHandler::throwError(ErrorCode Code, std::string ErrorInfo)
{
    AppPtr->raiseError(ErrorInfo);

    exit(static_cast<int>(Code));
}