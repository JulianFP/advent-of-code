#include "../fallingRocks.hh"
#include <iostream>
#include <termios.h>
#include <unistd.h>

int main(){
    //define rocks
    const std::vector<std::array<char, 10>> _rock1 = {{' ',' ','@','@','@','@',' ',' ',' ',' '}};
    const std::vector<std::array<char, 10>> _rock2 = {{' ',' ',' ','@',' ',' ',' ',' ',' ',' '},
                                                      {' ',' ','@','@','@',' ',' ',' ',' ',' '},
                                                      {' ',' ',' ','@',' ',' ',' ',' ',' ',' '}};
    const std::vector<std::array<char, 10>> _rock3 = {{' ',' ','@','@','@',' ',' ',' ',' ',' '},
                                                      {' ',' ',' ',' ','@',' ',' ',' ',' ',' '},
                                                      {' ',' ',' ',' ','@',' ',' ',' ',' ',' '}};
    const std::vector<std::array<char, 10>> _rock4 = {{' ',' ','@',' ',' ',' ',' ',' ',' ',' '},
                                                      {' ',' ','@',' ',' ',' ',' ',' ',' ',' '},
                                                      {' ',' ','@',' ',' ',' ',' ',' ',' ',' '},
                                                      {' ',' ','@',' ',' ',' ',' ',' ',' ',' '}};
    const std::vector<std::array<char, 10>> _rock5 = {{' ',' ','@','@',' ',' ',' ',' ',' ',' '},
                                                      {' ',' ','@','@',' ',' ',' ',' ',' ',' '}};
    const std::vector<std::vector<std::array<char, 10>>> rocks = {_rock1, _rock2, _rock3, _rock4, _rock5};
    // Save the current terminal attributes
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    // Set the terminal to raw mode
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fallingRocks<10> tetris(20, rocks);
    char nextchar;
    while(tetris.spawnNextRock(0)){
        do{
            tetris.print(std::cout, true);
            read(STDIN_FILENO, &nextchar, 1);
            if(nextchar == 'h') tetris.moveLeft();
            else if(nextchar == 'l') tetris.moveRight();
        }while(tetris.moveDown());
    }
    tetris.print(std::cout, true);

    // Restore the terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
