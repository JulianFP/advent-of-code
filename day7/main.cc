#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <cassert>

struct node{
    std::unordered_map<std::string, node*> children;
    std::string name;
    node* parent;
    int size = -1; //-1 means that size hasn't been calculated yet
    bool dir = 1; //if 0 node is leaf
};

std::array<std::string, 3> process_line(std::ifstream &file){
    std::string line;
    std::array<std::string, 3> result;
    std::getline(file, line);
    if(line[0] == '$'){
        result[0] = line[0];
        line.erase(line.begin(), line.begin()+2);
    }
    std::stringstream ss(line);
    ss >> result[1];
    ss >> result[2];
    return result;
}

void build_tree(std::string filename, node* root){
    std::ifstream file;
    file.open(filename);
    node* current_node = root;
    //0: $/empty, 1: command/dir/size, 2: path/empty/name
    std::array<std::string, 3> line;
    line = process_line(file);
    assert(line[0] == "$");
    while(!file.eof()){
        if(line[1] == "ls"){
            line = process_line(file);
            while((line[0] != "$")&&!file.eof()){
                node* new_node = new node;
                new_node->parent = current_node;
                new_node->name = line[2];
                if (line[1] != "dir"){
                    new_node->size = std::stoi(line[1]);
                    new_node->dir = 0;
                }
                std::pair<std::string, node*> child;
                child.first = line[2];
                child.second = new_node;
                current_node->children.insert(child);
                line = process_line(file);
            }
        }
        if(line[1] == "cd"){
            if(line[2] == "/"){
                current_node = root;
            }
            else if(line[2] == ".."){
                current_node = current_node->parent;
            }
            else{
                current_node = current_node->children.at(line[2]);
            }
            line = process_line(file);
            assert(line[0] == "$");
        }
    }
}

int calc_sizes(node* current_node){
    if(current_node->dir){
        current_node->size = 0;
        for(auto &child : current_node->children){
            current_node->size += calc_sizes(child.second);
        } 
        return current_node->size;
    }
    else return current_node->size;
}

int sum_of_dirs(node* current_node, int max_dir_size){
    if(current_node->dir){
        int sum = 0;
        if(current_node->size <= max_dir_size) sum = current_node->size;
        for(auto &child : current_node->children){
            sum += sum_of_dirs(child.second, max_dir_size);
        }
        return sum;
    }
    else return 0;
}

int find_dir_delete(node* current_node, int required_size){
    int smallest = current_node->size;
    for(auto &child : current_node->children){
        int childs = find_dir_delete(child.second, required_size);
         if((child.second->dir)&&(childs >= required_size)&&(childs < smallest)){
             smallest = childs;
         }
    }
    return smallest;
}

int main(){
    std::string filename;
    std::cout << "Enter name of your input file: " << std::flush;
    std::cin >> filename;
    node* root = new node;
    root->name = "/";
    build_tree(filename, root);
    for(auto entry : root->children){
        std::cout << entry.first << std::endl;
    }
    std::cout << calc_sizes(root) << std::endl;
    std::cout << "The solution for part 1 is: " << sum_of_dirs(root, 100000) << std::endl;
    int required_space = 30000000 - 70000000 + root->size;
    std::cout << "Theo solution for part 2 is: " << find_dir_delete(root, required_space) << std::endl;
    return 0;
}
