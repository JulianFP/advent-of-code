#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class TunnelSystem{
private:
    //properties of tunnel system and valves:
    std::unordered_map<std::string, int> _valves;   //translate valve names into numbers
    std::unordered_map<int, std::string> _valvesN;  //translates valve numbers into names
    std::vector<std::vector<int>> _adjList;         //adjacency list (graph representation)
    std::vector<int> _flowRate;                     //flow rate of valves
    int _numberOfValvesWithFlow = 0;                //amount of valves that have flowRate!=0

    //function that helps generating _valves
    int getNum(const std::string &ValveName, std::unordered_map<std::string, int> &valves){
        int ValveNum;
        if(valves.find(ValveName) == valves.end()){
            ValveNum = valves.size();
            valves.insert({ValveName, ValveNum});
        }
        else ValveNum = valves.at(ValveName);
        return ValveNum;
    }

    //stuff required for pressure Release calculation
    std::map<uint64_t, std::pair<int, std::vector<int>>> _visitedStates;
    int _startPosition;
    int _startMinutes;
    int _count = 0;
    inline int GetKey(const uint8_t &position, const uint8_t &minutes, const uint8_t &other_players, const std::vector<bool> &ValveOpen) const{
        uint64_t result = position | ((uint64_t)minutes << 8) | ((uint64_t)other_players << 16);
        for(int i = 0; i < ValveOpen.size(); ++i){
            result = result<<1 | (uint64_t)ValveOpen[i];
        }
        return result;
    }
    std::tuple<int, int, int, std::vector<bool>> GetValues(uint64_t key) const{
        std::tuple<int, int, int, std::vector<bool>> result;
        std::get<3>(result).resize(_numberOfValvesWithFlow);
        for(int i = std::get<3>(result).size()-1; i >= 0; --i){
            std::get<3>(result)[i] = (bool)(1 & key);
            key = key>>1;
        }
        std::get<0>(result) = (uint8_t)key;
        key = key>>8;
        std::get<1>(result) = (uint8_t)key;
        key = key>>8;
        std::get<2>(result) = (uint8_t)key;
        return result;
    }

    std::pair<int, std::vector<int>> pressureReleaseOfState(const uint8_t &last_position, const uint8_t &position, const uint8_t minutes, const uint8_t other_players, std::vector<bool> &ValveOpen){
        if(minutes == 0){
            if(other_players > 0) return pressureReleaseOfState(_startPosition, _startPosition, _startMinutes, other_players-1, ValveOpen);
            else return {0, std::vector<int>(0)};
        }
        uint64_t currentKey = GetKey(position, minutes, other_players, ValveOpen);
        auto Iterator = _visitedStates.find(currentKey);
        if(Iterator != _visitedStates.end()) return (*Iterator).second; 
        //std::cout << ++_count << "\n";

        std::pair<int, std::vector<int>> max;
        max.first = -1;
        if(_flowRate[position] > 0){
            if(!ValveOpen[position]){
                ValveOpen[position] = true;
                max = pressureReleaseOfState(position, position, minutes-1, other_players, ValveOpen); //add next pressure releases
                max.first += (minutes-1)*_flowRate[position]; //calculate total pressure that will be released by opened valve
                ValveOpen[position] = false;
            }
        }

        for(int i = 0; i < _adjList[position].size(); ++i){
            if(last_position != _adjList[position][i]){
                std::pair<int, std::vector<int>> current = pressureReleaseOfState(position, _adjList[position][i], minutes-1, other_players, ValveOpen);
                if(current.first > max.first){
                    max = current;
                }
            }
        }
        max.second.push_back(currentKey);
        _visitedStates.insert({currentKey, max});
        return max;
    }

public:
    TunnelSystem(std::string filename){
        std::ifstream file;
        file.open(filename);
        int ValveAmount = 0;
        std::unordered_map<std::string, int> valves;
        std::vector<std::vector<std::string>> adjList;
        std::vector<int> flowRate;
        while(!file.eof()){
            //read name of valve (at left side), get ValveNum and check for startValve
            file.ignore(100, ' ');
            std::string currentValve;
            std::getline(file, currentValve, ' ');
            if(currentValve.size() == 0) continue;
            int currentValveNum = getNum(currentValve, valves);
            flowRate.resize(valves.size());
            adjList.resize(valves.size());

            //read flowRate
            file.ignore(100, '=');
            file >> flowRate[currentValveNum];

            //read tunnels to other valves, get ValveNum and write into _adjList
            for(int i = 1; i <= 5; ++i){ //remove 5 words
                file.ignore(100, ' ');
            }
            std::string tunnelsString;
            std::getline(file, tunnelsString);
            std::stringstream tunnels(tunnelsString);
            while(!tunnels.eof()){
                std::string nextValve;
                std::getline(tunnels, nextValve, ',');
                int nextValveNum = getNum(nextValve, valves);
                flowRate.resize(valves.size());
                adjList.resize(valves.size());
                adjList[currentValveNum].push_back(nextValve);
                tunnels.ignore(1);
            }
        }
        file.close();

        if(valves.size() > 255) throw std::invalid_argument("Amount of valves has to be smaller than 256");
        _flowRate.resize(flowRate.size());
        _adjList.resize(adjList.size());
        for(const std::pair<std::string, int> &valve : valves){
            if(flowRate[valve.second] != 0){
                int currentValveNum = getNum(valve.first, _valves);
                _valvesN.insert({currentValveNum, valve.first});
                _flowRate[currentValveNum] = flowRate[valve.second];
                ++_numberOfValvesWithFlow;
            } 
        }
        if(_numberOfValvesWithFlow > 39) throw std::invalid_argument("Amount of valves with flow-rate!=0 has to be smaller than 40");
        for(const std::pair<std::string, int> &valve : valves){
            if(flowRate[valve.second] == 0){
                int currentValveNum = getNum(valve.first, _valves);
                _valvesN.insert({currentValveNum, valve.first});
                _flowRate[currentValveNum] = flowRate[valve.second];
            } 
        }
        for(const std::pair<std::string, int> &valve : valves){
            for(std::string &adj : adjList[valve.second]){
                _adjList[_valves.at(valve.first)].push_back(_valves.at(adj));
            }
        }
    }

    int maxReleasedPressure(std::string startValve, int startMinutes, int playerNumber){
        _count = 0;
        std::vector<bool> startOpenValves(_numberOfValvesWithFlow, false);
        _startPosition = _valves.at(startValve);
        _startMinutes = startMinutes;
        auto result = pressureReleaseOfState(_startPosition, _startPosition, _startMinutes, playerNumber-1, startOpenValves);
        for(int j = result.second.size()-1; j >= 0; --j){
            auto values = GetValues(result.second[j]);
            std::cout <<"\nPlayer: " << 2-std::get<2>(values) << " Minute: " << startMinutes-std::get<1>(values)+1 << " Position: " << _valvesN.at(std::get<0>(values)) << " Open Valves: ";
            for(int i = 0; i < std::get<3>(values).size(); ++i){
                if(std::get<3>(values)[i]) std::cout << _valvesN.at(i) << " ";
            }
        }
        std::cout << "\n";
        return result.first;
    }
};


int main(int argc, char **argv){
    std::string filename;
    if(argc > 1){
        filename = argv[1];
    }
    else{
        std::cout << "Enter filename of input: " << std::flush;
        std::cin >> filename;
    }
    TunnelSystem day16(filename);
    std::cout << "Result of part 1 is: " << day16.maxReleasedPressure("AA", 30, 1) << "\n";
    std::cout << "Result of part 2 is: " << day16.maxReleasedPressure("AA", 26, 2) << "\n";
    return 0;
}
