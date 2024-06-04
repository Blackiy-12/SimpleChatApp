#include <iostream>

#include "Arguments.hpp"
#include "App.hpp"

int main(int argc, char** argv)
{
    
    auto Arg = ArgumentParser::parseArguments(argc, argv);

    if (Arg.needToPrintHelp() == true)
    {
        Arg.printHelpInfo();
        return 0;
    }
    
    App Logic(Arg);
    Logic.start();
    
    return 0;
}