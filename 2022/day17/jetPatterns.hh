#pragma once
#include <vector>
#include <string>

class jetPatterns{
private:
    std::vector<char> _jetPattern;
    std::size_t _nextPattern = 0;
public:
    jetPatterns(const std::string &filename);

    char getNextPattern();

    int getPointer();
};

