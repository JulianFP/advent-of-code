#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

class cube{
private:
    std::array<int, 3> _coord;
public:
    cube() : _coord({0,0,0}){}
    cube(int x, int y, int z) : _coord({x,y,z}){}

    int& operator[](int it){
        return _coord[it];
    }

    bool operator<(const cube &other) const{
        for(int i = 0; i < _coord.size(); ++i){
            if(_coord[i] < other._coord[i]) return true;
            else if(other._coord[i] < _coord[i]) return false;
        }
        return false;
    }
};

class pane{
private:
    std::array<cube, 2> _cubes;
public:
    pane(const cube &first, const cube &second){
        if(first < second) _cubes = {first, second};
        else _cubes = {second, first};
    }

    pane() : _cubes({cube(), cube()}){}

    bool operator<(const pane &other) const{
        for(int i = 0; i < _cubes.size(); ++i){
            if(_cubes[i] < other._cubes[i]) return true;
            else if(other._cubes[i] < _cubes[i]) return false;
        }
        return false;
    }
};

void readInput(const std::string &filename, std::vector<cube> &result){
    std::ifstream input;
    input.open(filename);
    int x,y,z;
    std::string line;
    while(!input.eof()){
        std::getline(input, line);
        if(line.empty()) continue;
        std::stringstream linestream(line);
        linestream >> x;
        linestream.get();
        linestream >> y;
        linestream.get();
        linestream >> z;
        cube currentcube(x,y,z);
        result.push_back(currentcube);
    }
}

void getAdjaccentPanes(cube &cubeIn, std::vector<pane> &result){
    result.push_back(pane(cubeIn, cube(cubeIn[0]-1, cubeIn[1], cubeIn[2])));
    result.push_back(pane(cubeIn, cube(cubeIn[0]+1, cubeIn[1], cubeIn[2])));
    result.push_back(pane(cubeIn, cube(cubeIn[0], cubeIn[1]-1, cubeIn[2])));
    result.push_back(pane(cubeIn, cube(cubeIn[0], cubeIn[1]+1, cubeIn[2])));
    result.push_back(pane(cubeIn, cube(cubeIn[0], cubeIn[1], cubeIn[2]-1)));
    result.push_back(pane(cubeIn, cube(cubeIn[0], cubeIn[1], cubeIn[2]+1)));
}

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;

    std::vector<cube> cubes;
    readInput(filename, cubes);

    std::set<pane> panes;
    std::vector<pane> currentPanes;
    for(int i = 0; i < cubes.size(); ++i){
        currentPanes.resize(0);
        getAdjaccentPanes(cubes[i], currentPanes);
        for(int j = 0; j < currentPanes.size(); ++j){
            auto setPair = panes.insert(currentPanes[j]);
            if(!setPair.second) panes.erase(setPair.first);
        }
    }
    std::cout << "The result is " << panes.size() << std::endl;
}

