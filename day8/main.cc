#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

std::vector<std::vector<int>> read_file(std::string filename){
    std::ifstream file;
    file.open(filename);
    std::vector<std::vector<int>> result;
    std::string line;
    while(!file.eof()){
        std::getline(file, line);
        if(line.empty()) continue;
        std::vector<int> current_line(line.size());
        for(int i = 0; i < line.size(); i++){
            current_line[i] = int(line[i] - '0');
        } 
        result.push_back(current_line);
    }
    return result;
}

bool check_visible(const std::vector<std::vector<int>> &matrix, int row, int column){
    int height = matrix[row][column];
    int unvisible = 0;    //counts from how many sites the tree is not visible
    //checks if tree is unvisible to the left
    for(int i = 0; i < column; i++){
        if(matrix[row][i] >= height){
            ++unvisible;
            break;
        }; 
    }
    //checks if tree is unvisible to the right
    for(int i = column+1; i < matrix[row].size(); i++){
        if(matrix[row][i] >= height){
            ++unvisible;
            break;
        }; 
    }
    //checks if tree is unvisible to the top
    for(int i = 0; i < row; i++){
        if(matrix[i][column] >= height){
            ++unvisible;
            break;
        }; 
    }
    //checks if tree is unvisible to the bottom
    for(int i = row+1; i < matrix.size(); i++){
        if(matrix[i][column] >= height){
            ++unvisible;
            break;
        }; 
    }
    if(unvisible == 4) return false;
    else return true;
}

int check_scenic_score(const std::vector<std::vector<int>> &matrix, int row, int column){
    int height = matrix[row][column];
    int i, score = 1;
    //check left
    for(i = column-1; i > 0 && matrix[row][i] < height; --i){}
    if(i < 0) i = column;
    score *= column-i;
    //check right
    for(i = column+1; i < matrix[row].size()-1 && matrix[row][i] < height; ++i){}
    if(i > matrix[row].size()) i = column;
    score *= i-column;
    //check top
    for(i = row-1; i > 0 && matrix[i][column] < height; --i){}
    if(i < 0) i = column;
    score *= row-i;
    //check bottom
    for(i = row+1; i < matrix.size()-1 && matrix[i][column] < height; ++i){}
    if(i > matrix.size()) i =column;
    score *= i-row;
    return score; 
}

int count_visible(const std::vector<std::vector<int>> &matrix){
    int result = 0;
    for(int i = 0; i < matrix.size(); i++){ //iterates through rows
        for(int j = 0; j < matrix[i].size(); j++){  //iterates through columns
            if(check_visible(matrix, i, j)){
                ++result;
            };
        }
    }
    return result;
}

int highest_scenic_score(const std::vector<std::vector<int>> &matrix){
    int scenic_score = 0;
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            int current_score = check_scenic_score(matrix, i, j);
            if(current_score > scenic_score) scenic_score = current_score;
        }
    }
    return scenic_score;
}

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    std::vector<std::vector<int>> matrix = read_file(filename);
    std::cout << "solution for part 1 is: " << count_visible(matrix) << std::endl;
    std::cout << "solution for part 2 is: " << highest_scenic_score(matrix) << std::endl;
    return 0;
}
