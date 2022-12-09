#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <set>
#include <algorithm>

/*
    position[0]: x-coordinate of head
    position[1]: y-coordinate of head
    position[2]: x-coordinate of tail
    position[3]: y-coordinate of tail
    move[0]: new x-coordinate relative to old
    move[1]: new y-coordinate relative to old
*/

std::vector<std::array<int, 2>> read_file(std::string filename){
    std::ifstream file;
    file.open(filename);
    std::array<int, 2> move;
    std::vector<std::array<int, 2>> moves;
    char direction;
    int steps;
    std::string line;
    std::stringstream current_line(line);
    while(!file.eof()){
        std::getline(file, line);
        if(line.empty()) return moves;
        current_line.ignore();
        std::stringstream current_line(line);
        current_line >> direction;
        current_line >> steps;
        switch(direction){
            case 'R':
                move[0] = 1;
                move[1] = 0;
                break;
            case 'L':
                move[0] = -1;
                move[1] = 0;
                break;
            case 'U':
                move[0] = 0;
                move[1] = 1;
                break;
            case 'D':
                move[0] = 0;
                move[1] = -1;
        }
        for(int i = 1; i <= std::abs(steps); i++){
            moves.push_back(move); 
        }
    }
    file.close();
    return moves;
}

//calculate new tail position
std::vector<std::array<int, 2>> tail_correction(std::array<int, 4> &positions){
    int x_diff = positions[0]-positions[2];
    int y_diff = positions[1]-positions[3];
    std::vector<std::array<int, 2>> vec_of_move;
    std::array<int, 2> move;
    if(x_diff == 2){
        move[0] = 1;
        move[1] = 0;
        if(y_diff >= 1) move[1] = 1;
        else if(y_diff <= -1) move[1] = -1;
        vec_of_move.push_back(move);
    }
    else if(x_diff == -2){
        move[0] = -1;
        move[1] = 0;
        if(y_diff >= 1) move[1] = 1;
        else if(y_diff <= -1) move[1] = -1;
        vec_of_move.push_back(move);
    }
    else if(y_diff == 2){
        move[0] = 0;
        move[1] = 1;
        if(x_diff >= 1) move[0] = 1;
        else if(x_diff <= -1) move[0] = -1;
        vec_of_move.push_back(move);
    }
    else if(y_diff == -2){
        move[0] = 0;
        move[1] = -1;
        if(x_diff >= 1) move[0] = 1;
        else if(x_diff <= -1) move[0] = -1;
        vec_of_move.push_back(move);
    }
    return vec_of_move;
}

void moves_of_tail(std::vector<std::array<int, 2>> &moves){
    std::vector<std::array<int, 2>> moves_tail, new_moves_tail;
    std::array<int, 4> position = {0,0,0,0};
    for(std::array<int, 2> &move : moves){
        position[0] += move[0];
        position[1] += move[1];
        new_moves_tail = tail_correction(position);
        for(std::array<int, 2> &array : new_moves_tail){
            position[2] += array[0];
            position[3] += array[1];
        }
        std::copy(new_moves_tail.begin(), new_moves_tail.end(), std::back_inserter(moves_tail));
    }
    moves = moves_tail;
}

std::set<std::array<int, 2>> all_positions(std::vector<std::array<int, 2>> &moves){
    std::array<int, 2> position = {0,0}; //coordinates of only one knot
    std::set<std::array<int, 2>> all_positions;
    all_positions.insert(position);
    for(std::array<int, 2> &move : moves){
        position[0] += move[0]; 
        position[1] += move[1];
        all_positions.insert(position);
    }
    return all_positions;
}

void print_all_positions(std::set<std::array<int, 2>> all_positions){
    int y_largest = 0, y_smallest = 0, x_largest = 0, x_smallest = 0;
    for(std::array<int, 2> position : all_positions){
        if(position[0] > x_largest) x_largest = position[0];
        else if(position[0] < x_smallest) x_smallest = position[0]; 
        if(position[1] > y_largest) y_largest = position[1];
        else if(position[1] < y_smallest) y_smallest = position[1];
    }
    std::array<int, 2> current_position;
    std::array<int, 2> origin = {0,0}; 
    for(int i = y_largest; i >= y_smallest; i--){
        for(int j = x_smallest; j <= x_largest; j++){
            current_position = {j, i}; 
            auto it = all_positions.find(current_position);
            if(it != all_positions.end()){
                if(*it == origin) std::cout << 's';
                else std::cout << '#';
            }
            else std::cout << '.';
        }
        std::cout << std::endl;
    }
}

int main(){
    std::string filename;
    std::cout << "Enter file name: " << std::flush;
    std::cin >> filename;
    std::vector<std::array<int, 2>> moves = read_file(filename);
    std::set<std::array<int, 2>> positions = all_positions(moves);
    print_all_positions(positions);
    std::cout << "Positions of 1. knot (head). Number of all positions: " << positions.size() << '\n' << std::endl; 
    for(int i = 2; i <= 10; i++){
        moves_of_tail(moves);
        positions = all_positions(moves);
        print_all_positions(positions); 
        std::cout << "Positions of " << i << ". knot. Number of all positions: " << positions.size() << '\n' << std::endl; 
    }
    return 0;
}
