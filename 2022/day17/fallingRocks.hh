#ifndef FALLINGROCKS_HH
#define FALLINGROCKS_HH

#include <array>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

template<int width>
class fallingRocks{
private:
    /*
    ' ': position is empty
    '#': not-moving rock is at this position
    '@': moving rock is at this position
    */
    std::vector<std::array<char, width>> _cave;
    const std::vector<std::vector<std::array<char ,width>>> _rocks;
    int _nextRock = 0;
    int _oldPrintSize = 0;

    std::array<char, width> _emptyline;

    bool _checkEmpty(int line);
    void _makeNonMoving();

public:
    fallingRocks(int initialHeight, const std::vector<std::vector<std::array<char, width>>> &customRocks);

    /*
    newLines = 0: tries to spawn block inside existing cave without resizing it. returns false if not successful
    newLines > 0: resizes cave such that rock spawns above highest not moving rock (or ground) with newLines in between. Always returns true
    */
    bool spawnNextRock(int newLines);

    bool moveRight();
    bool moveLeft();
    bool moveDown();

    void print(std::ostream &outputStream, bool update);

    int getHeight();

    std::string getKey(int dependency);
};

template<int width>
fallingRocks<width>::fallingRocks(int initialHeight, const std::vector<std::vector<std::array<char, width>>> &customRocks) : _rocks(customRocks){
    _emptyline.fill(' ');
    _cave.resize(initialHeight, _emptyline);
}

template<int width>
bool fallingRocks<width>::_checkEmpty(int line){
    for(int col = 0; col < width; ++col){
        if(_cave[line][col] != ' ') return false;
    }
    return true;
}

template<int width>
void fallingRocks<width>::_makeNonMoving(){
    for(int line = 0; line < _cave.size(); ++line){
        for(int col = 0; col < width; ++col){
            if(_cave[line][col] == '@') _cave[line][col] = '#';
        }
    }
}

template<int width>
bool fallingRocks<width>::spawnNextRock(int newLines){
    if(newLines > 0){
        if(_cave.size() > 0) while(_checkEmpty(_cave.size()-1)) _cave.pop_back();
        _cave.resize(_cave.size() + newLines, _emptyline);
        for(int i = 0; i < _rocks[_nextRock].size(); ++i){
            _cave.push_back(_rocks[_nextRock][i]);
        }
    }
    else{
        for(int i = 0; i < _rocks[_nextRock].size(); ++i){
            _cave.push_back(_rocks[_nextRock][i]);
        }

        for(int i = 0; i < _rocks[_nextRock].size(); ++i){
            if(!moveDown()) return false;
        }
        _cave.resize(_cave.size() - _rocks[_nextRock].size());
    }
    _nextRock = (_nextRock + 1) % _rocks.size();
    return true;
}

template<int width>
bool fallingRocks<width>::moveRight(){
    auto tempCave = _cave;
    //iterate from right to left
    for(int col = width-1; col >= 0; --col){
        for(int line = 0; line < tempCave.size(); ++line){
            //check for moving rocks in cave
            if(tempCave[line][col] == '@'){

                //quit if moving rock is at most right position
                if(col + 1 == width) return false;

                //quit if the space at the right of a moving rock is occupied
                else if(tempCave[line][col+1] != ' ') return false;

                //else move moving rock to right
                else{
                    tempCave[line][col+1] = '@';
                    tempCave[line][col] = ' ';
                }
            }
        }
    }
    _cave = tempCave;
    return true;
}

template<int width>
bool fallingRocks<width>::moveLeft(){
    auto tempCave = _cave;
    //iterate from left to right
    for(int col = 0; col < width; ++col){
        for(int line = 0; line < tempCave.size(); ++line){
            //check for moving rocks in cave
            if(tempCave[line][col] == '@'){

                //quit if moving rock is at most left position
                if(col == 0) return false;

                //quit if the space at the left of a moving rock is occupied
                else if(tempCave[line][col-1] != ' ') return false;

                //else move moving rock to left
                else{
                    tempCave[line][col-1] = '@';
                    tempCave[line][col] = ' ';
                }
            }
        }
    }
    _cave = tempCave;
    return true;
}

template<int width>
bool fallingRocks<width>::moveDown(){
    auto tempCave = _cave;
    //iterate from bottom to top
    for(int line = 0; line < tempCave.size(); ++line){
        for(int col = 0; col < width; ++col){
            //check for moving rocks in cave
            if(tempCave[line][col] == '@'){

                //if moving rock is at most bottom position, make all moving rocks non-moving and quit
                if(line == 0){
                    _makeNonMoving();
                    return false;
                }

                //if the space at the bottom of a moving rock is occupied, make all moving rocks non-moving and quit
                else if(tempCave[line-1][col] != ' '){
                    _makeNonMoving();
                    return false;
                }

                //else move moving rock down
                else{
                    tempCave[line-1][col] = '@';
                    tempCave[line][col] = ' ';
                }
            }
        }
    }
    _cave = tempCave;
    return true;
}

template<int width>
void fallingRocks<width>::print(std::ostream &output, bool update){
    if(update && _oldPrintSize > 0){
        output << "\033[" << _oldPrintSize << "F";
    }

    //iterate from top to bottom
    for(int line = _cave.size()-1; line >= 0; --line){
        output << '|';
        for(int col = 0; col < width; ++col){
            output << _cave[line][col];
        }
        output << "|\n";
    }
    output << "+";
    for(int i = 0; i < width; ++i){
        output << "-";
    }
    output << "+\n" << std::endl; 

    _oldPrintSize = _cave.size() + 2;
}

template<int width>
int fallingRocks<width>::getHeight(){
    auto tempCave = _cave;
    if(tempCave.size() > 0) while(_checkEmpty(tempCave.size()-1)) tempCave.pop_back();
    return tempCave.size();
}

template<int width>
std::string fallingRocks<width>::getKey(int dependency){
    //calc shape of top of not-moving rocks
    std::array<int, width> indices;
    for(int col = 0; col < width; ++col){
        for(int line = _cave.size()-1; line >= 0; --line){
            if(_cave[line][col] == '#'){
                indices[col] = line + 1;
                break;
            }
            else if(line == 0){
                indices[col] = 0;
                break;
            }
        }
    }
    int* minElement = std::min_element(indices.begin(), indices.end());
    for(int i = 0; i < indices.size(); ++i){
        indices[i] -= *minElement;
    }

    //write all values into key and separate them with "#"
    std::string key;
    for(int i = 0; i < indices.size(); ++i){
        key.append(std::to_string(indices[i]) + "#");
    }
    key.append(std::to_string(_nextRock) + "#");
    key.append(std::to_string(dependency));

    return key;
}


#endif /* end of include guard: FALLINGROCKS_HH */
