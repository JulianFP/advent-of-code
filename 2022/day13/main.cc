#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>

//0: no decision, 1: right order, -1: wrong order
short checkPacketOrder(std::vector<int>::iterator leftIt, const std::vector<int>::iterator left_end, std::vector<int>::iterator rightIt, const std::vector<int>::iterator right_end){
    while(true){
        //check if a list has run out of items
        if((leftIt == left_end) && (rightIt == right_end)) return 0;
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
    short returnVal = checkPacketOrder(left.begin()+1, left.end()-1, right.begin()+1, right.end()-1);
    if(returnVal == -1) return 0;
    else return 1;
}

int main(){
    std::string filename;
    std::cout << "Enter name of file containing one pair of packets: " << std::flush;
    std::cin >> filename;
    std::ifstream file;
    file.open(filename);
    std::vector<int> currentVector;
    std::vector<std::vector<int>> packets;
    std::string current_line;
    char c;
    while(!file.eof()){
        std::getline(file, current_line);
        if(current_line.empty()) continue;
        std::stringstream line(current_line);
        int current = 0;
        int iterator = 0;
        currentVector.resize(0);
        while(line >> c){
            if(std::isdigit(c)){
                current *= 10;
                current += (int)c - 48;
                ++iterator;
            }
            else if(iterator != 0){
                currentVector.push_back(current); 
                current = 0;
                iterator = 0;
            }
            if(c == '[') currentVector.push_back(-1);
            else if(c == ']') currentVector.push_back(-2);
            else continue;
        }
        packets.push_back(currentVector);
    }
    file.close();
    int result = 0;
    for(int i = 0; i < packets.size(); i+=2){
        bool right_order = comparePackets(packets[i], packets[i+1]);
        result += ((i/2)+1)*right_order;
        std::cout << (i/2)+1 << ". pair of packets: " << right_order << std::endl;
    }
    std::cout << "The solution for part 1 is: " << result << std::endl;

    //part 2
    std::vector<int> divider_1 = {-1, -1, 2, -2, -2};
    std::vector<int> divider_2 = {-1, -1, 6, -2, -2};
    packets.push_back(divider_1);
    packets.push_back(divider_2);
    std::sort(packets.begin(), packets.end(), comparePackets);
    int result_2 = 1;
    for(int i = 0; i < packets.size(); i++){
        if(packets[i] == divider_1) result_2 *= (i+1);
        if(packets[i] == divider_2) result_2 *= (i+1);
        for(int &entry : packets[i]){
            std::cout << entry << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "The solution for part 2 is: " << result_2 << std::endl;
    return 0;
}
