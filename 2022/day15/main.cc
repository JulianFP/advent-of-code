#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <climits>
#include <set>
#include <algorithm>

//coordinate (easier to read than array)
struct point{
    int x;
    int y;
    point(int xin, int yin) : x(xin), y(yin){};
    point() : x(0), y(0){};
};

//stores coordinates of sensor and its nearest beacon
struct sensor{
    point coord; //coordinates of sensor
    point beacon; //coordinates of closest beacon
};

class day15_1{
    std::vector<sensor> _sensors; //list of all sensors

    long long int _calcTuningFrequency(point input) {
        long long int result = 4000000;
        result *= input.x;
        result += input.y;
        return result;
    }

    /*
    Writes all points of row y that cannot contain a beacon in input
    This is done by storing sections of those points defined by an array<int, 2>
    The first element of this array stores the beginning of a section, the second the end of a section
    */
    void _cannotContain(std::vector<std::array<int,2>> &input, int y){
        for(sensor &dsensor : _sensors){
            //calculate distance by Manhattan metric
            unsigned int sensorBeacon_distance = std::abs(dsensor.coord.x - dsensor.beacon.x) + std::abs(dsensor.coord.y - dsensor.beacon.y);
            unsigned int sensorMidPoint_distance = std::abs(y - dsensor.coord.y);
            if(sensorMidPoint_distance > sensorBeacon_distance){
                continue;
            }
            else{
                int half_SectorWidth = sensorBeacon_distance - sensorMidPoint_distance;
                std::array<int, 2> newSector = {dsensor.coord.x - half_SectorWidth, dsensor.coord.x + half_SectorWidth};
                input.push_back(newSector);
            }
        }
        std::sort(input.begin(), input.end());
        int i = 0;
        while(i+1 < input.size()){
            if(input[i][1] >= input[i+1][0]){
                input[i][1] = std::max(input[i][1], input[i+1][1]);
                input.erase(input.begin()+i+1);
            }
            else{
                ++i;
            }
        }
    }

    public:
    day15_1(std::string filename){
        std::ifstream file;
        file.open(filename);
        sensor current_sensor;
        while(!file.eof()){
            file.ignore(100, '=');
            file >> current_sensor.coord.x;
            file.ignore(100, '=');
            file >> current_sensor.coord.y;
            file.ignore(100, '=');
            file >> current_sensor.beacon.x;
            file.ignore(100, '=');
            file >> current_sensor.beacon.y;
            file.ignore(5);
            _sensors.push_back(current_sensor);
        }
    }

    //counts all points from the output of _cannotContain (minus those which already contain a beacon)
    int countCannotContain(int y){
        int result = 0;
        std::vector<std::array<int, 2>> *dontContain = new std::vector<std::array<int, 2>>;
        _cannotContain(*dontContain, y);
        for(std::array<int, 2> &section : *dontContain){
            result += section[1] - section[0]; 
        }
        std::set<int> checkedBeacons; //needed because several sensors can have same beacon
        for(sensor &dsensor : _sensors){
            if(dsensor.beacon.y == y && checkedBeacons.find(dsensor.beacon.x) == checkedBeacons.end()){
                --result;
                checkedBeacons.insert(dsensor.beacon.x);
            }
        }
        return result;
    }

    /*
    checks area defined by minX,maxX,minY,maxY for all possible positions
    of distress frequencies and outputs a vector of their tuning frequencies
    */
    std::vector<long long int> canContain(int minX, int maxX, int minY, int maxY){
        std::vector<long long int> result;
        for(int y = minY; y <= maxY; y++){
            std::vector<std::array<int, 2>> sectionsCanContain;
            std::vector<std::array<int, 2>> *dontContain = new std::vector<std::array<int, 2>>;
            _cannotContain(*dontContain, y);
            std::array<int, 2> masterSection = {minX, maxX};
            for(std::array<int, 2> &section : *dontContain){
                if(section[0] > masterSection[0]){
                    sectionsCanContain.push_back({masterSection[0], section[0]-1});
                }
                if(section[1] < masterSection[1]){
                    masterSection[0] = section[1]+1;
                }
                else break;
            }
            for(std::array<int, 2> &section : sectionsCanContain){
                for(int i = section[0]; i <= section[1]; i++){
                    point thisPoint = {i, y};
                    result.push_back(_calcTuningFrequency(thisPoint));
                }
            }
        }
        return result;
    }
};


int main(){
    std::string filename;
    int y;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    std::cout << "For part 1: Enter the y of the desired line: " << std::flush;
    std::cin >> y;
    day15_1 bothParts(filename);
    std::cout << "The solution for part 1 is: " << bothParts.countCannotContain(y) << std::endl;
    int max;
    std::cout << "For part 2: Enter the maximal value for x and y: " << std::flush;
    std::cin >> max;
    std::vector<long long int> allPossiblePos = bothParts.canContain(0, max, 0, max);
    std::cout << "The solution for part 2 is: ";
    for(long long int &freq : allPossiblePos){
        std::cout << freq << " ";
    }
    std::cout << std::endl;
    return 0;
}
