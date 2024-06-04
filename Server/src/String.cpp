#include "String.hpp"
#include <sstream>

SplittedString::SplittedString(std::string Str)
{
    splitString(Str);
}

SplittedString::~SplittedString()
{
}

void SplittedString::splitString(std::string Str)
{
    std::istringstream iss(Str);
    std::string word;
    
    while (iss >> word) 
    {
        Words.push_back(word);
    }
}

int SplittedString::getWordsCount() const
{
    return Words.size();
}

std::string SplittedString::getWord(int WordNumber) const
{
    if (WordNumber >= int(Words.size()))
        return std::string("");
    return Words[WordNumber];
}
