#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <list>

/*
    cycle[0]: register value during first cycle
    cycle[1]: register value during second cycle
    ...
*/

std::vector<int> build_cycle(std::string filename){
    std::ifstream file;
    file.open(filename);
    std::string line;
    std::stringstream ss;
    int addx;
    std::vector<int> cycle(1,1);
    while(!file.eof()){
        std::getline(file, line);
        if(line.empty()) continue;
        else if(line == "noop"){
            cycle.push_back(cycle.back()); 
        }
        else{
            ss.ignore();
            std::stringstream ss(line);
            ss.ignore(5);
            ss >> addx;
            cycle.push_back(cycle.back()); 
            cycle.push_back(cycle.back()+addx);
        }
    }
    return cycle;
}

int sum_of_strengths(std::vector<int> &cycle, std::list<int> interested_cycles){
    int sum = 0;
    for(int &cycle_num : interested_cycles){
        sum += cycle[cycle_num-1]*cycle_num;
    }
    return sum;
}

void render_image(std::vector<int> &cycle){
    int min_pix, max_pix, row = 0;
    for(int cycle_num = 0; cycle_num < cycle.size()-1; cycle_num++){
        min_pix = cycle[cycle_num]-1;
        max_pix = cycle[cycle_num]+1;
        if((cycle_num % 40 >= min_pix) && cycle_num % 40 <= max_pix){
            std::cout << '#';
        }
        else std::cout << '.';
        if((cycle_num+1)/40 > row){
            std::cout << '\n';
            ++row;
        }
    }
    std::cout << std::endl;
}

int main(){
    std::string filename;
    std::cout << "Enter name of file: " << std::flush; 
    std::cin >> filename;
    std::list<int> interested_cycles = {20, 60, 100, 140, 180, 220};
    std::vector<int> cycle = build_cycle(filename);
    std::cout << "Solution for part 1: " << sum_of_strengths(cycle, interested_cycles) << std::endl;
    std::cout << "Solution for part 2 (rendered image): " << std::endl;
    render_image(cycle);
    return 0;
}
