#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool check_if_marker(const char *buffer, int size){
    for(int i = 0; i < size; i++){
        for(int j = i+1; j < size; j++){
            if(buffer[i] == buffer[j]) return false; 
        }
    }
    return true;
}

int result_1(std::string filename, int size){
    std::ifstream file;
    file.open(filename);
    int iterator = size;
    char buffer[size+1], new_char;
    file.get(buffer, size+1);
    while(!file.eof()){
        if(check_if_marker(buffer, size)){
            file.close();
            return iterator;
        }
        else{
            for(int i = 0; i < size-1; i++){
                buffer[i] = buffer[i+1];
            }
            file >> new_char;
            buffer[size-1] = new_char;
            ++iterator;
        }

    }
    file.close();
    return -1; //error
}

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    std::cout << "The result of part 1 is: " << result_1(filename, 4) << std::endl;
    std::cout << "The result for part 2 is: " << result_1(filename, 14) << std::endl;
    return 0;
}
