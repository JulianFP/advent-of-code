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
    part2: Instead of storing the worry_levels, store instead the remainders of the divisions in queue items
*/

struct monkey{
    std::queue<std::vector<int>> items;
    std::array<int, 2> operation = {1, 0};
    std::array<int, 3> test;
};

class day11{
    std::vector<monkey> monkeys;
    std::vector<long long int> inspected_items;

    void convert_worry_levels(){
        for(monkey &Monkey : monkeys){
            for(int i = 0; i < Monkey.items.size(); i++){
                std::vector<int> current_item = Monkey.items.front();
                current_item.resize(monkeys.size());
                int old_worry = current_item[0];
                for(int j = 0; j < monkeys.size(); j++){
                    current_item[j] = old_worry % monkeys[j].test[0];
                }
                Monkey.items.pop();
                Monkey.items.push(current_item);
            }
        }
    }

    void calculate_new_worry_level(std::vector<int> &old_level, int monkey_num){
        for(int i = 0; i < old_level.size(); i++){
            if(monkeys[monkey_num].operation[0] == 0) old_level[i] *= old_level[i];
            else{
                old_level[i] *= monkeys[monkey_num].operation[0];
                old_level[i] += monkeys[monkey_num].operation[1];
            }
            old_level[i] = old_level[i] % monkeys[i].test[0];
        }
    }
    
    public:
    day11(std::string filename){ //read file, each scope {} is for one line
        std::ifstream file;
        file.open(filename);
        std::string line;
        int sign;
        char char_buffer;
        std::vector<int> item(1);
        while(!file.eof()){
            std::getline(file, line);
            monkey new_monkey;
            {
                std::getline(file, line); 
                line.erase(line.begin(), line.begin()+18);
                std::stringstream ss(line); 
                while(!ss.eof()){
                    ss >> item[0];
                    new_monkey.items.push(item);
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
        convert_worry_levels();
        file.close();
        inspected_items.resize(monkeys.size());
    }

    void throw_items(int rounds){
        for(int i = 1; i <= rounds; i++){
            for(int j = 0; j < monkeys.size(); j++){
                while(!monkeys[j].items.empty()){
                    std::vector<int> new_worry = monkeys[j].items.front();
                    monkeys[j].items.pop();
                    calculate_new_worry_level(new_worry, j);
                    if(new_worry[j] == 0) monkeys[monkeys[j].test[1]].items.push(new_worry);
                    else monkeys[monkeys[j].test[2]].items.push(new_worry);
                    ++inspected_items[j];
                }
            }
        }
    } 

    long long int monkey_business(){
        std::vector<long long int> sorted_inspected_items = inspected_items;
        std::sort(sorted_inspected_items.begin(), sorted_inspected_items.end());
        long long int biggest = sorted_inspected_items.back();
        sorted_inspected_items.pop_back();
        long long int second_biggest = sorted_inspected_items.back();
        return biggest*second_biggest;
    }

};


int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    /*
    day11 part1(filename);
    part1.throw_items(20, true);
    std::cout << "The solution for part 1 is: " << part1.monkey_business() << std::endl;
    */
    day11 part2(filename);
    part2.throw_items(10000);
    std::cout << "The solution for part 2 is: " << part2.monkey_business() << std::endl;
    return 0;
}
