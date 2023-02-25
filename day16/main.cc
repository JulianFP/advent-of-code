#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class TunnelSystem{
private:
    //state is dependent of this three parameters
    struct state{
        const int minutes;
        const int position;
        const std::vector<bool> ValveOpen;
        state(const int &min, const int &pos, const std::vector<bool> &open) : minutes(min), position(pos), ValveOpen(open){}

        bool operator<(const state &other) const{
            if(minutes < other.minutes) return true;
            else if(other.minutes < minutes) return false;
            else if(position < other.position) return true;
            else if(other.position < position) return false;
            else if(ValveOpen < other.ValveOpen) return true;
            else return false;
        }

        bool operator>(const state &other) const{
            return other < *this;
        }

        bool operator==(const state &other) const{
            if((minutes == other.minutes) && (position == other.position) && (ValveOpen == other.ValveOpen)) return true;
            else return false;
        }
    };
    const int _startMinutes;
    int _startPosition;

    //translate valve names to numbers
    std::unordered_map<std::string, int> _valves;
    int getNum(std::string ValveName){
        int ValveNum;
        if(_valves.find(ValveName) == _valves.end()){
            ValveNum = _flowRate.size();
            _valves.insert({ValveName, ValveNum});
            _flowRate.push_back(0);
            _adjList.resize(_adjList.size()+1);
        }
        else ValveNum = _valves.at(ValveName);
        return ValveNum;
    }

    //adjacency list
    std::vector<std::vector<int>> _adjList;

    //flow rate of valves
    std::vector<int> _flowRate;


    int pressureReleaseInMinute(const std::vector<bool> &valveOpen) const{
        int returnVal = 0;
        for(int i = 0; i < _valves.size(); ++i){
            if(valveOpen[i]) returnVal += _flowRate[i];
        }
        return returnVal;
    }

    std::map<state, int> _releasedPressureOfState;

    int _counter = 0;

    int pressureReleaseOfState(const int last_position, const int minutes, const int position, std::vector<bool> valveOpen){
        if(minutes == 0) return 0;
        state currentState(minutes, position, valveOpen);
        auto Iterator = _releasedPressureOfState.find(currentState); 
        if(Iterator != _releasedPressureOfState.end()) return (*Iterator).second; 
        std::cout << ++_counter << "\n";

        int max = 0;
        if((!valveOpen[position]) && (_flowRate[position] > 0)){
            valveOpen[position] = true;
            max = pressureReleaseOfState(position, minutes-1, position, valveOpen); 
            valveOpen[position] = false;
        }

        for(int i = 0; i < _adjList[position].size(); ++i){
            if(last_position != _adjList[position][i])
            max = std::max(max, pressureReleaseOfState(position, minutes-1, _adjList[position][i], valveOpen));
        }
        int result = pressureReleaseInMinute(valveOpen) + max;
        _releasedPressureOfState.insert({currentState, result});
        return result;
    }

public:
    TunnelSystem(std::string filename, std::string startValve, int startMinutes) : _startMinutes(startMinutes){
        std::ifstream file;
        file.open(filename);
        int ValveAmount = 0;
        while(!file.eof()){
            //read name of valve (at left side), get ValveNum and check for startValve
            file.ignore(100, ' ');
            std::string currentValve;
            std::getline(file, currentValve, ' ');
            if(currentValve.size() == 0) continue;
            int currentValveNum = getNum(currentValve);
            if(currentValve == startValve){
                _startPosition = currentValveNum;
            }

            //read flowRate
            file.ignore(100, '=');
            file >> _flowRate[currentValveNum];

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
                int nextValveNum = getNum(nextValve);
                _adjList[currentValveNum].push_back(nextValveNum);
                tunnels.ignore(1);
            }
        }
    }

    int maxReleasedPressure(){
        std::vector<bool> startOpenValves(_valves.size(), false);
        return pressureReleaseOfState(_startPosition, _startMinutes, _startPosition, startOpenValves);
    }
};


int main(){
    std::string filename;
    std::cout << "Enter filename of input: " << std::flush;
    std::cin >> filename;
    TunnelSystem day16(filename, "AA", 30);
    std::cout << "Result of part 1 is: " << day16.maxReleasedPressure() << "\n";
    return 0;
}
