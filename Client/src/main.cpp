#include <iostream>

#include "Arguments.hpp"
#include "App.hpp"
#include "ErrorHandle.hpp"

int main(int argc, char** argv)
{
    
    auto Arg = ArgumentParser::parseArguments(argc, argv);

    if (Arg.needToPrintHelp() == true)
    {
        Arg.printHelpInfo();
        return 0;
    }

    if (Arg.isCompleted() == false)
    {
        ErrorHandle::throwError("Uncompleted arguments");
        exit(98);
    }
    
    App Logic(Arg);
    Logic.startLoop();
    
    return 0;
}