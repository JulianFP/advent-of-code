#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <queue>
#include <algorithm>

/*
    operation[0]: Value to multiply with old
    operation[1]: Value to add to old
    if operation == {0,0}, then it means old*old
    divisible by test[0]. If true: throw to monkey test[1]. If false: throw to monkey test[2]
*/

struct monkey{
    std::queue<int> items;
    std::array<int, 2> operation = {1, 0};
    std::array<int, 3> test;
};

class day11{
    std::vector<monkey> monkeys;
    std::vector<int> inspected_items;

    public:
    day11(std::string filename){ //read file, each scope {} is for one line
        std::ifstream file;
        file.open(filename);
        std::string line;
        int buffer, sign;
        char char_buffer;
        while(!file.eof()){
            std::getline(file, line);
            monkey new_monkey;
            {
                std::getline(file, line); 
                line.erase(line.begin(), line.begin()+18);
                std::stringstream ss(line); 
                while(!ss.eof()){
                    ss >> buffer;
                    new_monkey.items.push(buffer);
                    ss.ignore(2);
                }
            }
            {
                std::getline(file, line);
                line.erase(line.begin(), line.begin()+23);
                std::stringstream ss(line);
                ss >> char_buffer;
                if(char_buffer == '*') sign = 0;
                else sign = 1;
                ss >> new_monkey.operation[sign]; 
            }
            {
            std::getline(file, line);
            line.erase(line.begin(), line.begin()+21);
            std::stringstream ss(line);
            ss >> new_monkey.test[0];
            }
            {
                std::getline(file, line);
                line.erase(line.begin(), line.begin()+29);
                std::stringstream ss(line);
                ss >> new_monkey.test[1];
            }
            {
                std::getline(file, line);
                line.erase(line.begin(), line.begin()+30);
                std::stringstream ss(line);
                ss >> new_monkey.test[2];
            }
            std::getline(file, line);
            monkeys.push_back(new_monkey);
        }
        file.close();
        inspected_items.resize(monkeys.size());
    }

    void throw_items(int rounds){
        for(int i = 1; i <= rounds; i++){
            for(int j = 0; j < monkeys.size(); j++){
                while(!monkeys[j].items.empty()){
                    int new_worry = monkeys[j].items.front();
                    monkeys[j].items.pop();
                    if(monkeys[j].operation[0] == 0) new_worry *= new_worry;
                    else{
                        new_worry *= monkeys[j].operation[0];
                        new_worry += monkeys[j].operation[1];
                    }
                    new_worry = new_worry/3;
                    if(new_worry % monkeys[j].test[0] == 0) monkeys[monkeys[j].test[1]].items.push(new_worry);
                    else monkeys[monkeys[j].test[2]].items.push(new_worry);
                    ++inspected_items[j];
                }
            }
        }
    } 

    int monkey_business(){
        std::vector<int> sorted_inspected_items = inspected_items;
        std::sort(sorted_inspected_items.begin(), sorted_inspected_items.end());
        int biggest = sorted_inspected_items.back();
        sorted_inspected_items.pop_back();
        int second_biggest = sorted_inspected_items.back();
        return biggest*second_biggest;
    }

};


int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    day11 input(filename);
    input.throw_items(20);
    std::cout << "The solution for part 1 is: " << input.monkey_business() << std::endl;
    return 0;
}
