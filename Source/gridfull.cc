#include "../Headers/gridfull.h"
#include "../Headers/claim.h"
#include <algorithm> // find vector
#include <iostream> // testing
#include <time.h> // sleep for testing

/*  
    UPDATE:
    Used vectors
*/

/*
    NODE-TYPE CONSTANTS:
    kSink = 1
    kSource = 0
    kFree = -1
    kBlock = -2
    kVistited = -3
    kBorder = -4
*/
const int kSink = 1;
const int kSource = 0;
const int kFree = -1;
const int kBlock = -2;
const int kVisited = -3;
const int kBorder = -4;

// inherit constructor from Grid class
Utilities::GridFull::GridFull(ProblemObject* problem_object){
    int height = problem_object->get_height();
    int width = problem_object->get_width();
    for(int y = 0; y < height; y++) {
        vector<NodeFull*> temp_row;
        for(int x = 0; x < width; x++){
            temp_row.push_back(new NodeFull(x, y));
        }
        this->grid.push_back(temp_row);
    }
    
    include_blockers(problem_object);
    check_connections(problem_object);
    // print_grid();
    //TODO
}

int Utilities::GridFull::get_width() {
	//Assumes a perfect rectangle
	return grid.empty()?0:grid.at(0).size();
}

int Utilities::GridFull::get_height() {
	//Assumes a perfect rectangle
	return this->grid.size();
}

inline void testCode(){
    string pause;
    std::getline(std::cin, pause);
    if(pause == "q"){
        exit(1);
    }
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
                grid.at(y).at(x)->set_status(kBlock);
            }
        }
    }
}

// Ensure that connection's location are within the grid's limits
void Utilities::GridFull::check_connections(ProblemObject* problem_object){
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
        NodeFull* current_source = grid.at(source_y).at(source_x);
        NodeFull* current_sink = grid.at(sink_y).at(sink_x);
        // This code relies on the fact that blockers are already added onto grid_nodes
        if(current_source->get_status() == kBlock){
            claim("Connection " + name + " source is in a blocker", kError);
            return;
        }
        if(current_source->get_status() == kBlock){
            claim("Connection " + name + " sink is in a blocker", kError);
            return;
        }
        
        // Insert connection's source and sink nodes into queue
        connections.push(std::pair<NodeFull*, NodeFull*> (current_source, current_sink));
    }
}

Utilities::NodeFull* Utilities::GridFull::load_connection(){
    connections.front().first->set_status(kSource);
    connections.front().second->set_status(kSink);
    return connections.front().first;
}

void Utilities::GridFull::print_grid(){
    int width = get_width();    // decrease call cost
    for(int y = get_height() - 1; y >= 0; --y){
        for(int x = 0; x < width - 1; ++x){
            NodeFull* current_node = grid.at(y).at(x);
            if(current_node->get_status() == kFree){
                // the current_node is free/empty;
                std::cout << '.';
            }
            else if(current_node->get_status() == kBlock){
                // the current_node is a block
                std::cout << '~';
            }
            else if(current_node->get_status() >= 0){
                // assign a number to the source
                std::cout << current_node->get_status();
            }
            else if(current_node->get_status() == kVisited){
                std::cout << current_node->get_cost() % 10;
            }
            else if(current_node->get_status() == kBorder){
                std::cout << 'B';
            }
        }
        std::cout << endl;
    }
}

void Utilities::GridFull::run_lee(int sink_num){
    // input the first node (source) into queue
    NodeFull* sink_node = load_connection();
    border.push(sink_node);
    
    bool path_found = false;
    while(!border.empty()){
        
        // TEST
        // testCode();
        
        NodeFull* front = border.front();
        // start searching
        if(search_north(front) || search_east(front) || search_south(front)
            || search_west(front)){
                path_found = true;
                break;    
        }
        border.front()->set_status(kVisited);
        border.pop();
        
        // TEST
        // print_grid();
    }
    
    sink_node->set_status(kSource);
}

bool Utilities::GridFull::search_north(NodeFull* border_node){
    int north_y = border_node->get_y() + 1;
    int north_x = border_node->get_x();
    int cost = border_node->get_cost() + 1;
    if(north_y< get_height()){
        // node is in grid range
        return increment_path(grid.at(north_y).at(north_x),cost); 
    }
    return false;
}

bool Utilities::GridFull::search_west(NodeFull* border_node){
    int west_y = border_node->get_y();
    int west_x = border_node->get_x() - 1;
    int cost = border_node->get_cost() + 1;
    if(west_x >= 0){
        // Western node is within grid limits
        return increment_path(grid.at(west_y).at(west_x),cost);
    }
    return false;
}

bool Utilities::GridFull::search_south(NodeFull* border_node){
    int south_y = border_node->get_y() - 1;
    int south_x = border_node->get_x();
    int cost = border_node->get_cost() + 1;
    if(south_y >= 0){
        // Southern node is within grid limits
        return increment_path(grid.at(south_y).at(south_x),cost);
    }
    return false;
}

bool Utilities::GridFull::search_east(NodeFull* border_node){
    int east_y = border_node->get_y();
    int east_x = border_node->get_x() + 1;
    int cost = border_node->get_cost() + 1;
    if(east_x < get_width()){
        // Eastern node is within grid limits
        return increment_path(grid.at(east_y).at(east_x),cost);
    }
    return false;
}

bool Utilities::GridFull::increment_path(NodeFull* new_border_node, int cost){
    if(new_border_node->get_status() == kSink){
        
        claim("Found Path!", kNote);
        return true;
    }
    if(new_border_node->get_status() >= kFree){
        new_border_node->set_status(kBorder);
        new_border_node->set_cost(cost);
        border.push(new_border_node);
    }
    return false;
}

// void Utilities::GridFull::percolate(Node* source){
    
// }


