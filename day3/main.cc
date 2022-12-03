#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::vector<char>> read_file(std::string filename){
    std::ifstream file;
    file.open(filename);
    std::vector<std::vector<char>> result;
    std::vector<char> current_line;
    std::string line_string;
    while(!file.eof()){
        current_line.resize(0);
        line_string.resize(0);
        std::getline(file, line_string);
        for(int i = 0; i < line_string.size(); i++){
            int priority = 0; 
            if(!std::isalpha(line_string[i])) continue;
            current_line.push_back(line_string[i]);
        }
        if (!current_line.empty()) result.push_back(current_line);
    }
    file.close();
    return result;
}

std::vector<char> check_same(std::vector<std::vector<char>> input){
    std::vector<char> result;
    for(std::vector<char> &entry : input){
        for(int i = 0; i < entry.size()/2; i++){
            for(int j = entry.size()/2; j < entry.size(); j++){
                if(entry[i] == entry[j]){ 
                    result.push_back(entry[i]);
                    goto breakpoint;
                }
            }
        }
        breakpoint:
        continue;
    }
    return result;
}

std::vector<char> check_same_2(std::vector<std::vector<char>> input){
    std::vector<char> result;
    for(int i = 0; i < input.size(); i += 3){
        for(char &entry : input[i]){
            for(char &entry2 : input[i+1]){
                if(entry == entry2){
                    for(char &entry3 : input[i+2]){
                        if(entry3 == entry2){
                            result.push_back(entry3);
                            goto breakpoint;
                        }
                    }
                }
            }
        }
        breakpoint:
        continue;
    }
    return result;
}

int sum(std::vector<char> same){
    int result = 0;
    for(char &entry : same){
        int priority = 0;
        //check if letter is upper-case (in ASCII 6th bit has to be 0)
        if((entry & 32) == 0) priority += 26; 
        priority += (entry & 31); 
        result += priority;
    }
    return result;
}

int main(){
    std::string filename;
    std::cout << "Enter name of your input file: " << std::flush;
    std::cin >> filename; 
    std::cout << "Result for part 1: " << sum(check_same(read_file(filename))) << std::endl;; 
    std::cout << "Result for part 2: " << sum(check_same_2(read_file(filename))) << std::endl;
    return 0;
}
