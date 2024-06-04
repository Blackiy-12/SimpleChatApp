#include "ErrorHandle.hpp"

#include <csignal>

#include "IO.hpp"
#include "Config.hpp"
#include "App.hpp"

App* ErrorHandle::AppPtr = nullptr;

void ErrorHandle::throwError(std::string ErrorMessage)
{
    if (Config::getConfig()->isAppInError() == true)
        return;
    
    Config::getConfig()->setAppInError();
    IO::printInternalError(ErrorMessage);
}

void ErrorHandle::throwErrorAndExit(std::string ErrorMessage, int ExitCode)
{
    if (Config::getConfig()->isAppInError() == true)
        return;
    
    Config::getConfig()->setAppInError();
    IO::printInternalError(ErrorMessage);
    
    AppPtr->exitApp(ExitCode);
}


void ErrorHandle::linkApp(App* AppPtr)
{
    ErrorHandle::AppPtr = AppPtr;
}