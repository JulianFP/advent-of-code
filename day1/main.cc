#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

std::vector<int> read_file(std::string filename){
    std::ifstream input;
    input.open(filename);
    std::vector<int> sums; 
    int iterator = 0;
    while(!input.eof()){
        std::string line;
        std::getline(input, line);
        if(line != ""){
            if(sums.size() < iterator+1) sums.resize(2*(iterator+1));
            sums[iterator] += std::stoi(line); 
        }
        else ++iterator;
    }
    return sums;
}

std::pair<int, int> largest_sum(std::vector<int>& sums){
    std::pair<int, int> largest = {0,-1};
    for(int i = 0; i < sums.size(); i++){
        if(sums[i] > largest.first){
            largest.first = sums[i];
            largest.second = i;
        };
    }
    return largest;
}

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    std::vector<int> calories = read_file(filename);
    int sum_of_sums = 0;
    int amountofsums = 3;
    for(int i = 0; i < amountofsums; i++){
        std::pair<int, int> last_largest = largest_sum(calories);
        std::cout << i << ". largest sum: " << last_largest.first << "\n";
        calories.erase(calories.begin()+last_largest.second);
        sum_of_sums += last_largest.first;
    }
    std::cout << "sum of the " << amountofsums << " largest sums is: " << sum_of_sums << std::endl;
}
