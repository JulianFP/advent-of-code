#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <array>

std::pair<std::vector<std::stack<char>>, std::vector<std::array<int, 3>>> read_file(std::string filename){
    //first: Vector of starting stacks, second: vector of arrays with move instructions
    std::pair<std::vector<std::stack<char>>, std::vector<std::array<int, 3>>> result; 
    std::ifstream file;
    file.open(filename);
    std::string current_line;
    std::vector<std::string> stack_lines;

    //read stacks by iterating from bottom to top
    do{
        std::getline(file, current_line);
        stack_lines.push_back(current_line);
    }while(!current_line.empty());
    stack_lines.pop_back();
    int stack_count = ((stack_lines.back().size()-3)/4)+1;
    result.first.resize(stack_count);
    for(int i = stack_lines.size()-2; i >= 0; i--){
        for(int j = 0; j < stack_lines[i].size(); j++){
            if (std::isalpha(stack_lines[i][j])) result.first[(j-1)/4].push(stack_lines[i][j]); 
        }
    }

    //read instructions
    std::array<int, 3> line;
    while(!file.eof()){
        line.fill(-1);
        std::getline(file, current_line);
        for(char &c : current_line){
            if(std::isalpha(c)) c = ' ';
        }
        std::stringstream ss(current_line);
        ss >> line[0];
        ss >> line[1];
        ss >> line[2];
        if(line[2] != -1) result.second.push_back(line);
    }
    return result; 
}

void rearrange(std::pair<std::vector<std::stack<char>>, std::vector<std::array<int, 3>>> &input){
    for(std::array<int, 3> &array : input.second){
        for(int i = 0; i < array[0]; i++){
            input.first[array[2]-1].push(input.first[array[1]-1].top());
            input.first[array[1]-1].pop();
        }
    }
}

void rearrange_2(std::pair<std::vector<std::stack<char>>, std::vector<std::array<int, 3>>> &input){
    for(std::array<int, 3> &array : input.second){
        std::stack<char> crane;
        for(int i = 0; i < array[0]; i++){
            crane.push(input.first[array[1]-1].top());
            input.first[array[1]-1].pop();
        }
        while(!crane.empty()){
            input.first[array[2]-1].push(crane.top());
            crane.pop();
        }
    }
}

std::string output(const std::pair<std::vector<std::stack<char>>, std::vector<std::array<int, 3>>> &input){
    std::string result;
    for(const std::stack<char> &stack : input.first){
        result.push_back(stack.top());
    }
    return result;
}

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    std::pair<std::vector<std::stack<char>>, std::vector<std::array<int, 3>>> result_1 = read_file(filename);
    auto result_2 = result_1;
    rearrange(result_1);
    rearrange_2(result_2);
    std::cout << "The solution for part 1 is: " << output(result_1) << std::endl;
    std::cout << "The solution for part 1 is: " << output(result_2) << std::endl;
    return 0;
}
