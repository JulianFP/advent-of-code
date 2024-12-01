#include "jetPatterns.hh"
#include <fstream>

jetPatterns::jetPatterns(const std::string &filename){
    //read file
    std::ifstream file;
    file.open(filename);
    std::string pattern;
    std::getline(file, pattern);
    file.close();

    //read jetPattern
    for(int i = 0; i < pattern.size(); ++i){
        if(pattern[i] == '<') _jetPattern.push_back('h');
        else if(pattern[i] == '>') _jetPattern.push_back('l');
        else break;
    }
}

char jetPatterns::getNextPattern(){
    char returnVal = _jetPattern[_nextPattern];
    _nextPattern = (_nextPattern + 1) % _jetPattern.size();
    return returnVal;
}

int jetPatterns::getPointer(){
    return _nextPattern;
}
