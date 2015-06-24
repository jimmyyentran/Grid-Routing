#include "../Headers/gridfull.h"
#include "../Headers/claim.h"
#include <algorithm> // find vector
#include <iostream> // testing

/*  
    This version of GridFull utilizes maps<Node*, int> for more efficient
    search time than gridfull_old which searches through sets and vectors 
    iteratively to find Node*'s status. Maps keep all data within one data member 
    for simplicity and faster lookup time.
*/

/*
    NODE-TYPE CONSTANTS:
    kBlock = -1
    kVistited = -2
    kBorder = -3
    kFree = -4
    
    All sinks and sources are non-negatives. 
    Sources are even
    Sinks are odds and sources + 1
    
    Ex. Source1 = 0, Sink1 = 1. Source2 = 2, Sink2 = 3.
*/

const int kBlock = -1;
const int kVisited = -2;
const int kBorder = -3;
const int kFree = -4;

// inherit constructor from Grid class
Utilities::GridFull::GridFull(ProblemObject* problem_object) : Grid(problem_object) {
    include_blockers(problem_object);
    include_connections(problem_object);
    print_grid();
    //TODO
}

// add blockers into the grid. Blocker's initial Point is at top left corner
void Utilities::GridFull::include_blockers(ProblemObject* problem_object){
    vector<Blocker> block = problem_object->get_blockers();
    int height = problem_object->get_height();
    int width = problem_object->get_width();
    for(int i = 0; i < block.size(); i++){
        
        // Ensure that blocker's location and size are within the grid's limits
        string name = block.at(i).name;
        int block_width = block.at(i).width;
        int block_height = block.at(i).height;
        int block_x = block.at(i).location.x;
        int block_y = block.at(i).location.y;
        
        if(block_x >= width || block_x < 0){
            claim("Blocker " + name + " location must be within range from 0 to (grid's width - 1)", kError);
            return;
        }
        if(block_y >= height || block_y < 0){
            claim("Blocker " + name + " location must be within range from 0 to (grid's height - 1)", kError);
            return;
        }
        if(block_width + block_x >= width){
            claim("Blocker " + name + " width extends beyond the grid's width", kError);
            return;
        }
        if(block_height > block_y){
            claim("Blocker " + name + " height extends beyond the grid's height", kError);
            return;
        }
        
        // insert blocked nodes into set beginning from top left corner
        for(int y = block_y; y > block_y - block_height; --y){
            for(int x = block_x; x < block_x + block_width; ++x){
                // TEST
                // std::cout << "x: " << x << "    y: " << y << endl;
                // blocked_nodes.insert(get_node(x, y));
                grid_nodes[get_node(x, y)] = kBlock;
            }
        }
    }
}

//Ensure that connection's location are within the grid's limits
void Utilities::GridFull::include_connections(ProblemObject* problem_object){
    vector<Connection> connection = problem_object->get_connections();
    int height = problem_object->get_height();
    int width = problem_object->get_width();
    int connection_counter = 0;
    for(int i = 0; i < connection.size(); i++){
        
        //Ensure that connection's location are within the grid's limits
        string name = connection.at(i).name;
        int source_x = connection.at(i).source.x;
        int source_y = connection.at(i).source.y;
        int sink_x = connection.at(i).sink.x;
        int sink_y = connection.at(i).sink.y;
        
        if(source_x >= width || source_x < 0){
            claim("Connection " + name + " source must be within range from 0 to (grid's width - 1)", kError);
            return;
        }
        if(source_y >= height || source_y < 0){
            claim("Connection " + name + " source must be within range from 0 to (grid's height - 1)", kError);
            return;
        }
        if(sink_x >= width || sink_x < 0){
            claim("Connection " + name + " sink must be within range from 0 to (grid's width - 1)", kError);
            return;
        }
        if(sink_y >= height || sink_y < 0){
            claim("Connection " + name + " sink must be within range from 0 to (grid's height - 1)", kError);
            return;
        }
        
        //Ensure source and sink are not inside blocks
        Node* current_source = get_node(source_x, source_y);
        Node* current_sink = get_node(sink_x, sink_y);
        
        // This code relies on the fact that blockers are already added onto grid_nodes
        if(grid_nodes.find(current_source) != grid_nodes.end()){
            if(grid_nodes.find(current_source)->second == kBlock){
                claim("Connection " + name + " source is in a blocker", kError);
                return;
            }
        }
        if(grid_nodes.find(current_sink) != grid_nodes.end()){
            if(grid_nodes.find(current_sink)->second == kBlock){
                claim("Connection " + name + " sink is in a blocker", kError);
                return;
            }
        }
        
        // Insert connection's source and sink nodes
        grid_nodes[current_source] = connection_counter++;
        grid_nodes[current_sink] = connection_counter++;
    }
}

void Utilities::GridFull::print_grid(){
    //TODO: need to add priting for visited and border
    int width = get_width();    // decrease call cost
    int connection_counter = 0;
    for(int y = get_height() - 1; y >= 0; --y){
        for(int x = 0; x < width - 1; ++x){
            Node* current_node = get_node(x, y);
            if(grid_nodes.find(current_node) == grid_nodes.end()){
                // the current_node is free/empty;
                std::cout << " ";
            }
            else if(grid_nodes.find(current_node)->second == kBlock){
                // the current_node is a block
                std::cout << 'X';
            }
            else if(grid_nodes.find(current_node)->second >= 0){
                // assign a number to the source
                std::cout << grid_nodes.find(current_node)->second;
            }
            else if(grid_nodes.find(current_node)->second >= 0){
                // assign a number to the sink
                std::cout << grid_nodes.find(current_node)->second;
            }
        }
        std::cout << endl;
    }
}

void Utilities::GridFull::lee_step(int sink_num){
    
}

// void Utilities::GridFull::percolate(Node* source){
    
// }


