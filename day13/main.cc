#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

//0: no decision, 1: right order, -1: wrong order
short checkPacketOrder(std::vector<int>::iterator leftIt, const std::vector<int>::iterator left_end, std::vector<int>::iterator rightIt, const std::vector<int>::iterator right_end){
    while(true){
        //check if a list has run out of items
        if((leftIt == left_end) && (rightIt != right_end)) return 1;
        else if((rightIt == right_end) && (leftIt != left_end)) return -1; 

        //check if both values are a list
        else if((*leftIt == -1) && (*rightIt == -1)){
            std::vector<int>::iterator leftBegin = leftIt+1, leftEnd = leftIt, rightBegin = rightIt+1, rightEnd = rightIt;
            int it = 1;
            do{
                ++leftEnd;
                if(*leftEnd == -1) ++it;
                if(*leftEnd == -2) --it;
            }while(it != 0);
            it = 1;
            do{
                ++rightEnd;
                if(*rightEnd == -1) ++it;
                if(*rightEnd == -2) --it;
            }while(it != 0);
            short recResult = checkPacketOrder(leftBegin, leftEnd, rightBegin, rightEnd);
            if(recResult == 0){
                leftIt = leftEnd+1;
                rightIt = rightEnd+1;
            }
            else return recResult;
        }

        //check if one value is list and other is int
        else if(*leftIt == -1){
            std::vector<int>::iterator leftBegin = leftIt+1, leftEnd = leftIt;
            int it = 1;
            do{
                ++leftEnd;
                if(*leftEnd == -1) ++it;
                if(*leftEnd == -2) --it;
            }while(it != 0);
            short recResult = checkPacketOrder(leftBegin, leftEnd, rightIt, rightIt+1);
            if(recResult == 0){
                leftIt = leftEnd+1;
                ++rightIt;
            }
            else return recResult;
        }
        else if(*rightIt == -1){
            std::vector<int>::iterator rightBegin = rightIt+1, rightEnd = rightIt;
            int it = 1;
            do{
                ++rightEnd;
                if(*rightEnd == -1) ++it;
                if(*rightEnd == -2) --it;
            }while(it != 0);
            short recResult = checkPacketOrder(leftIt, leftIt+1, rightBegin, rightEnd);
            if(recResult == 0){
                rightIt = rightEnd+1;
                ++leftIt;
            }
            else return recResult;
        }

        //else both values have to be int since -2 is always right_end
        else{
            if(*leftIt > *rightIt) return -1;
            else if(*leftIt < *rightIt) return 1;
            ++leftIt;
            ++rightIt;
        }
    }
    return 0;
}

bool comparePackets(std::vector<int> left, std::vector<int> right){
    short returnVal = checkPacketOrder(left.begin(), left.end()-1, right.begin(), right.end()-1);
    if(returnVal == -1) return 0;
    else return 1;
}

int main(){
    std::string filename;
    std::cout << "Enter name of file containing one pair of packets: " << std::flush;
    std::cin >> filename;
    std::ifstream file;
    file.open(filename);
    std::vector<int> oldVector, newVector;
    std::string current_line;
    char c;
    int pair = 0, result = 0;
    while(!file.eof()){
        std::getline(file, current_line);
        if(current_line.empty()) continue;
        std::stringstream line(current_line);
        int current = 0;
        int iterator = 0;
        oldVector = newVector;
        newVector.resize(0);
        while(line >> c){
            if(std::isdigit(c)){
                current *= 10;
                current += (int)c - 48;
                ++iterator;
            }
            else if(iterator != 0){
                newVector.push_back(current); 
                current = 0;
                iterator = 0;
            }
            if(c == '[') newVector.push_back(-1);
            else if(c == ']') newVector.push_back(-2);
            else continue;
        }
        ++pair;
        if(pair%2 == 0){
            bool right_order = comparePackets(oldVector, newVector);
            result += (pair/2)*right_order;
            std::cout << (pair/2) << ". pair of packets: " << right_order << std::endl;
        }
    }
    std::cout << "The solution for part 1 is: " << result << std::endl;
    file.close();
    return 0;
}
