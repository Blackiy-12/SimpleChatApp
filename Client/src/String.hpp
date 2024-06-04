#pragma once
#include <string>
#include <vector>

class SplittedString
{
public:
    SplittedString(std::string Str);
    ~SplittedString();

    int getWordsCount() const;

    std::string getWord(int WordNumber) const;

private:
    std::vector<std::string> Words;

    void splitString(std::string Str);
};
