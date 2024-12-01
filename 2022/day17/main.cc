#include "fallingRocks.hh"
#include "jetPatterns.hh"
#include <iostream>
#include <string>
#include <unordered_map>

std::array<long long, 2> getLoopIterator(const std::array<int, 2> &oldValue, const std::array<int, 2> &newValue, long long iterations){
    std::array<long long, 2> returnVal;

    int loopSize = newValue[0] - oldValue[0];
    long long loopAm = (iterations - oldValue[0]) / loopSize;
    returnVal[0] = oldValue[0] + (loopAm * loopSize);

    int loopHeight = newValue[1] - oldValue[1];
    returnVal[1] = oldValue[1] + (loopAm * loopHeight);

    return returnVal;
}

int main(){
    //define rocks
    const std::vector<std::array<char, 7>> _rock1 = {{' ',' ','@','@','@','@',' '}};
    const std::vector<std::array<char, 7>> _rock2 = {{' ',' ',' ','@',' ',' ',' '},
                                                     {' ',' ','@','@','@',' ',' '},
                                                     {' ',' ',' ','@',' ',' ',' '}};
    const std::vector<std::array<char, 7>> _rock3 = {{' ',' ','@','@','@',' ',' '},
                                                     {' ',' ',' ',' ','@',' ',' '},
                                                     {' ',' ',' ',' ','@',' ',' '}};
    const std::vector<std::array<char, 7>> _rock4 = {{' ',' ','@',' ',' ',' ',' '},
                                                     {' ',' ','@',' ',' ',' ',' '},
                                                     {' ',' ','@',' ',' ',' ',' '},
                                                     {' ',' ','@',' ',' ',' ',' '}};
    const std::vector<std::array<char, 7>> _rock5 = {{' ',' ','@','@',' ',' ',' '},
                                                     {' ',' ','@','@',' ',' ',' '}};
    const std::vector<std::vector<std::array<char, 7>>> rocks = {_rock1, _rock2, _rock3, _rock4, _rock5};

    std::string filename;
    long long int iterations;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    std::cout << "Enter amount of iterations: " << std::flush;
    std::cin >> iterations;
    jetPatterns jet(filename);

    fallingRocks<7> tetris(0, rocks);
    char nextchar;
    std::array<long long, 2> afterLoop = {-1, -1};
    std::array<int, 2> value;
    {
        std::string key;
        std::unordered_map<std::string, std::array<int, 2>> hashMap;
        for(int i = 1; i <= iterations; ++i){
            value = {i, tetris.getHeight()};
            key = tetris.getKey(jet.getPointer());
            auto retPair = hashMap.insert(std::make_pair(key, value));
            if(!retPair.second){
                afterLoop = getLoopIterator(retPair.first->second, value, iterations);
                break;
            }
            tetris.spawnNextRock(3);
            do{
                nextchar = jet.getNextPattern();
                if(nextchar == 'h') tetris.moveLeft();
                else if(nextchar == 'l') tetris.moveRight();
            }while(tetris.moveDown());

        }
    }
    if(afterLoop[0] != -1){
        for(long long i = afterLoop[0]; i <= iterations; ++i){
            tetris.spawnNextRock(3);
            do{
                nextchar = jet.getNextPattern();
                if(nextchar == 'h') tetris.moveLeft();
                else if(nextchar == 'l') tetris.moveRight();
            }while(tetris.moveDown());
        }
    }
    tetris.print(std::cout, false);
    std::cout << "The result is " << (tetris.getHeight() - value[1]) + afterLoop[1] << std::endl;

    return 0;
}
