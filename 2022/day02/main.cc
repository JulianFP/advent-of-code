#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::pair<std::vector<char>, std::vector<char>> read_file(std::string filename){
    std::ifstream file;
    file.open(filename);
    std::vector<char> opp, me;
    while(!file.eof()){
        char current = ' ';
        file >> current;
        opp.push_back(current);
        file >> current;
        me.push_back(current);
    }
    std::pair<std::vector<char>, std::vector<char>> rvalue = {opp, me};
    return rvalue;
}

int round_score(char my_move, char opp_move){
    int my_score;
    if(my_move == 'X') my_score = 1;
    else if(my_move == 'Y') my_score = 2;
    else if(my_move == 'Z') my_score = 3;
    if(my_move == 'X' && opp_move == 'C') my_score += 6;
    else if(my_move == 'Y' && opp_move == 'A') my_score += 6;
    else if(my_move == 'Z' && opp_move == 'B') my_score += 6;
    else if(my_move == 'X' && opp_move == 'A') my_score += 3;
    else if(my_move == 'Y' && opp_move == 'B') my_score += 3;
    else if(my_move == 'Z' && opp_move == 'C') my_score += 3;
    return my_score;
}

int round_score_part2(char result, char opp_move){
    int my_score;
    if(result == 'Z'){
        my_score = 6;
        if(opp_move == 'A') my_score += 2;
        else if(opp_move == 'B') my_score += 3;
        else if(opp_move == 'C') my_score += 1;
    }
    else if(result == 'Y'){
        my_score = 3;
        if(opp_move == 'A') my_score += 1;
        else if(opp_move == 'B') my_score += 2;
        else if(opp_move == 'C') my_score += 3; 
    }
    else if(result == 'X'){
        my_score = 0;
        if(opp_move == 'A') my_score += 3;
        if(opp_move == 'B') my_score += 1;
        if(opp_move == 'C') my_score += 2;
    }
    return my_score;
}

int calculate_score(std::pair<std::vector<char>, std::vector<char>> input, int (*roundscore)(char, char)){
    int sum = 0;
    for(int i = 0; i < input.first.size(); i++){
        if(!std::isalpha(input.first[i]) || !std::isalpha(input.second[i])) continue;
        sum += roundscore(input.second[i], input.first[i]);
    }
    return sum;
}

int main(){
    std::cout << "Enter name of input file: " << std::flush;
    std::string filename;
    std::cin >> filename;
    std::cout << "The result is: " << calculate_score(read_file(filename), round_score) << std::endl;
    std::cout << "The result for part 2 is: " << calculate_score(read_file(filename), round_score_part2) << std::endl;
    return 0;
    
}
