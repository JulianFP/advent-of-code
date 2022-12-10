#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>

std::vector<std::array<int, 4>> read_file(std::string filename){
    std::vector<std::array<int, 4>> result;
    std::ifstream file;
    file.open(filename);
    std::array<int, 4> current_line;
    while(!file.eof()){
        char bin;
        current_line.fill(-1);
        for(int &entry : current_line){
            file >> entry;
            file.get(bin);
        }
        if(current_line[0] != -1) result.push_back(current_line);
    }
    file.close();
    return result;
}

int range_contain_count(std::vector<std::array<int, 4>> input){
    int result = 0;
    for(std::array<int, 4> &array : input){
        //check if first range contains second
        if((array[0] <= array[2]) && (array[1] >= array[3])){
            ++result;
            continue;
        };
        //check if second range contains first
        if((array[2] <= array[0]) && (array[3] >= array[1])) ++result;
    }
    return result;
}

int range_overlap(std::vector<std::array<int, 4>> input){
    int result = 0;
    for(std::array<int, 4> &array : input){
        //first range under second: check if first reaches into second
        if((array[0] <= array[2]) && (array[1] >= array[2])){
            ++result;
            continue;
        };
        //second range under first: check if seoncd reaches into first
        if((array[2] <= array[0]) && (array[3] >= array[0])) ++result;
    }
    return result;

}

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename; 
    std::vector<std::array<int, 4>> input = read_file(filename);
    std::cout << "The solution for part 1 is: " << range_contain_count(input) << std::endl;
    std::cout << "The solution for part 2 is: " << range_overlap(input) << std::endl;
    return 0;
}
