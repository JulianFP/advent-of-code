#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <set>
#include <climits>

class day11{
    //store height_map
    std::vector<std::vector<char>> _height_map;

    /*
    array: stores bool values for each element in matrix indicating if a path is traversable
    array[0]: up, array[1]: down, array[2]: left, array[3]: right
    vector of these arrays is a row
    vector of these rows is whole matrix
    */
    std::vector<std::vector<std::array<bool, 4>>> _height_graph;

    public:
    //store coordinates of start and end (row, column)
    std::array<int, 2> start, end; 

    day11(std::string filename){
        //read from file to set _height_map, _start and _end
        std::ifstream file;
        file.open(filename);
        std::string line;
        for(int row = 0; !file.eof(); row++){
            std::getline(file, line);
            if(line.empty()) continue;
            _height_map.resize(_height_map.size()+1);
            _height_map[row].resize(line.size());
            for(int column = 0; column < line.size(); column++){
                if(line[column] == 'S'){
                    start = {row, column};
                    _height_map[row][column] = 'a';
                }
                else if(line[column] == 'E'){
                    end = {row, column};
                    _height_map[row][column] = 'z';
                }
                else _height_map[row][column] = line[column];
            }
        }
        file.close();

        //generate _height_graph
        _height_graph.resize(_height_map.size());
        for(auto &vector : _height_graph){
            vector.resize(_height_map[0].size());
        }
        int diff;
        for(int row = 0; row < _height_graph.size(); row++){
            for(int column = 0; column < _height_graph[row].size(); column++){
                if(column < _height_graph[row].size()-1){
                    diff = (_height_map[row][column]&31)-(_height_map[row][column+1]&31);
                    if(diff >= -1) _height_graph[row][column][3] = 1;
                    if(diff <= 1) _height_graph[row][column+1][2] = 1; 

                }
                if(row < _height_graph.size()-1){
                    diff = (_height_map[row][column]&31)-(_height_map[row+1][column]&31);
                    if(diff >= -1) _height_graph[row][column][1] = 1;
                    if(diff <= 1) _height_graph[row+1][column][0] = 1; 
                }
            }
        }
    }

    //performs breadth-first search beginning at _start and returns depth where _end was encountered
    int bfs(std::array<int, 2> starting_point){
        std::set<std::array<int, 2>> B = {starting_point}, new_B, visited = {starting_point};
        int depth = 0;
        while(B.count(end) == 0){
            for(const std::array<int, 2> &array : B){
                if(_height_graph[array[0]][array[1]][0]){
                    if(visited.count({array[0]-1, array[1]}) == 0){
                        new_B.insert({array[0]-1, array[1]});
                        visited.insert({array[0]-1, array[1]});
                    }
                }
                if(_height_graph[array[0]][array[1]][1]){
                    if(visited.count({array[0]+1, array[1]}) == 0){
                        new_B.insert({array[0]+1, array[1]});
                        visited.insert({array[0]+1, array[1]});
                    }
                }
                if(_height_graph[array[0]][array[1]][2]){
                    if(visited.count({array[0], array[1]-1}) == 0){
                        new_B.insert({array[0], array[1]-1});
                        visited.insert({array[0], array[1]-1});
                    }
                }
                if(_height_graph[array[0]][array[1]][3]){
                    if(visited.count({array[0], array[1]+1}) == 0){
                        new_B.insert({array[0], array[1]+1});
                        visited.insert({array[0], array[1]+1});
                    }
                }
            }
            if(new_B.empty()) return INT_MAX;
            B = new_B;
            new_B.clear();
            ++depth;
        }
        return depth;
    }

    //performs bfs for every a and outputs smallest result
    int bfs_all_a(){
        int smallest = INT_MAX, current;
        std::array<int, 2> Coord;
        for(int row = 0; row < _height_map.size(); row++){
            for(int column = 0; column < _height_map[row].size(); column++){
                Coord = {row, column};
                if(_height_map[row][column] == 'a'){
                    current = bfs(Coord);
                    if(current < smallest) smallest = current;
                } 
            }
        }
        return smallest;
    }
};

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;
    day11 solution(filename);
    std::cout << "The solution for part1 is: " << solution.bfs(solution.start) << std::endl;
    std::cout << "The solution for part2 is: " << solution.bfs_all_a() << std::endl;
    return 0;
}
