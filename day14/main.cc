#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <array>
#include <string>

/*
    std::deque: List of columns, at column[0] is y=0
    _maxRow: max y-coordinate stored in columns (or: size of columns - 1)
    _minCol: x-coordinate of deque.front() (first element in deque)
    _maxCol: x-coordinate of deque.back() (last element in deque)
    std::array<int, 2> = {x,y}
*/

class day14{
    protected:
    std::deque<std::vector<char>> _matrix;
    int _maxRow, _minCol, _maxCol;
    bool _full = false; //stores if configuration is full of sand

    /*
    reads a coordinate from line. line.tellg() has to be right before first number of coordinate 
    processes following characters: "<int>,<int> -> " (example: "498,4 -> ")
    after this function line.tellg() is right before first number of next coordinate again or line.eof() is set
    */
    std::array<int, 2> _read_coord(std::stringstream &line){
        std::array<int, 2> result;
        line >> result[0];
        char c;
        while(!std::isdigit(line.peek())){
            line.get(c);
        }
        line >> result[1];
        while(!std::isdigit(line.peek()) && !line.eof()){
            line.get(c);
        }
        return result;
    }

    //draws a line from firstPoint to secondPoint into the matrix
    void _draw_line(std::array<int, 2> firstPoint, std::array<int, 2> secondPoint){
        int minX = std::min(firstPoint[0], secondPoint[0]);
        int maxX = std::max(firstPoint[0], secondPoint[0]);
        int minY = std::min(firstPoint[1], secondPoint[1]);
        int maxY = std::max(firstPoint[1], secondPoint[1]);
        //resize _matrix and its columns
        if(maxY > _maxRow){
            for(std::vector<char> &column : _matrix){
                column.resize(maxY+1, '.');
            }
            _maxRow = maxY;
        }
        while(minX < _minCol){
            std::vector<char> newCol(_maxRow+1, '.');
            _matrix.push_front(newCol);
            --_minCol;
        }
        while(maxX > _maxCol){
            std::vector<char> newCol(_maxRow+1, '.');
            _matrix.push_back(newCol);
            ++_maxCol;
        }
        //draw line (if it is on y-axis)
        if(minX == maxX){
            for(int i = minY; i <= maxY; i++){
                _matrix[minX-_minCol][i] = '#';
            }
        }
        //draw line (if it is on x-axis)
        if(minY == maxY){
            for(int i = minX; i <= maxX; i++){
                _matrix[i-_minCol][minY] = '#';
            }
        }
    }

    //outputs a new column for matrix
    virtual std::vector<char> _newCol(){
        std::vector<char> newCol(_maxRow+1, '.');
        return newCol;
    }

    //checks if Col is outside matrix in which case it adds new columns
    void _extendMatrix(int Col){
        std::vector<char> newCol = _newCol();
        while(Col > _maxCol){
            _matrix.push_back(newCol);
            ++_maxCol;
        }
        while(Col < _minCol){
            _matrix.push_front(newCol);
            --_minCol;
        }
    }

    //checks if coord is blocked by '#' or 'o'
    bool _blocked(std::array<int, 2> coord){
        if(coord[0] < _minCol || coord[0] > _maxCol){
            _extendMatrix(coord[0]);
        }
        if(coord[1] > _maxRow) return false;
        if(_matrix[coord[0]-_minCol][coord[1]] == '.') return false;
        else if(_matrix[coord[0]-_minCol][coord[1]] == '+') return false;
        else return true;
    }

    public:
    //read file and build matrix:
    day14(std::string filename) : _matrix({{'+'}}), _maxRow(0), _minCol(500), _maxCol(500) {
        std::string currentLine;
        std::ifstream file;
        std::array<int, 2> firstCoord, secondCoord;
        file.open(filename);
        while(!file.eof()){
            std::getline(file, currentLine);
            if(currentLine.empty()) continue;
            std::stringstream line(currentLine);
            secondCoord = _read_coord(line);
            while(!line.eof()){
                firstCoord = secondCoord;
                secondCoord = _read_coord(line);
                _draw_line(firstCoord, secondCoord);
            }
        }
        file.close();
    }

    //outputs matrix in std::cout
    void outputMatrix(){
        for(int row = 0; row <= _maxRow; row++){
            for(int column = _minCol; column <= _maxCol; column++){
                std::cout << _matrix[column-_minCol][row];
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

    //drops one piece of sand until it comes to rest or falls into void
    bool dropSand(){
        if(_full) return true;
        std::array<int, 2> currentCrd, nextCrd = {500, 0};
        if(_blocked(nextCrd)){
            _full = true;
            return true;
        }
        std::vector<std::array<int, 2>> visited;
        while(currentCrd != nextCrd){
            if(nextCrd[1] > _maxRow){
                _full = true;
                for(std::array<int, 2> &visitedCord : visited){
                    _matrix[visitedCord[0] - _minCol][visitedCord[1]] = '~';
                }
                return true;
            }
            currentCrd = nextCrd;
            visited.push_back(currentCrd);
            ++nextCrd[1];
            if(_blocked(nextCrd)){
                --nextCrd[0];
                if(_blocked(nextCrd)){
                    nextCrd[0] += 2;
                    if(_blocked(nextCrd)){
                        nextCrd = currentCrd;
                    }
                }
            }
        }
        _matrix[currentCrd[0]-_minCol][currentCrd[1]] = 'o';
        return false;
    }
};

class day14_2 : public day14{
    protected:
    //draws floor under matrix (gets called after generating matrix)
    void _drawFloor(){
        for(std::vector<char> &column : _matrix){
            column.push_back('.');
            column.push_back('#');
        }
        _maxRow += 2;
    }

    //new columns have to include floor
    std::vector<char> _newCol() override{
        std::vector<char> newCol(_maxRow, '.');
        newCol.push_back('#');
        return newCol;
    }

    public:
    day14_2(std::string filename) : day14(filename){
        _drawFloor();
    }
};

int main(){
    std::string filename;
    std::cout << "Enter name of input file: " << std::flush;
    std::cin >> filename;

    //part 1:
    day14 part1(filename);
    part1.outputMatrix();
    int part1_result = 0;
    while(!part1.dropSand()){
        ++part1_result;
        part1.outputMatrix();
    }
    part1.outputMatrix();
    std::cout << "The solution for part 1 is: " << part1_result << std::endl;

    //part 2, code here same as part1:
    day14_2 part2(filename);
    part2.outputMatrix();
    int part2_result = 0;
    while(!part2.dropSand()){
        ++part2_result;
        //part2.outputMatrix();
    }
    part2.outputMatrix();
    std::cout << "The solution for part 2 is: " << part2_result << std::endl;
    return 0;
}
