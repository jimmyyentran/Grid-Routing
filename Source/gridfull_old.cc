#include "../Headers/gridfull.h"
#include "../Headers/claim.h"
#include <algorithm> // find vector
#include <iostream> // testing

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
                blocked_nodes.insert(get_node(x, y));
            }
        }
    }
}

//Ensure that connection's location are within the grid's limits
void Utilities::GridFull::include_connections(ProblemObject* problem_object){
    vector<Connection> connection = problem_object->get_connections();
    int height = problem_object->get_height();
    int width = problem_object->get_width();
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
        if(blocked_nodes.find(current_source) != blocked_nodes.end()){
            claim("Connection " + name + " source is in a blocker", kError);
            return;
        }
        if(blocked_nodes.find(current_sink) != blocked_nodes.end()){
            claim("Connection " + name + " sink is in a blocker", kError);
            return;
        }
        
        // Insert connection's source and sink nodes
        source.push_back(current_source);
        sink.push_back(current_sink);
    }
}

void Utilities::GridFull::print_grid(){
    int width = get_width();    // decrease call cost
    for(int y = get_height() - 1; y >= 0; --y){
        for(int x = 0; x < width - 1; ++x){
            Node* current_node = get_node(x, y);
            typename vector<Node*>::iterator source_it = std::find(source.begin(), source.end(), current_node);
            typename vector<Node*>::iterator sink_it = std::find(sink.begin(), sink.end(), current_node);
            if(blocked_nodes.find(current_node) != blocked_nodes.end()){
                std::cout << "X";
            }
            else if(source_it != source.end()){
                // assign a number to the the source and the sink
                std::cout << std::distance(source.begin(), source_it);
            }
            else if(sink_it != sink.end()){
                std::cout << std::distance(sink.begin(), sink_it);
            }
            else{
                std::cout << " ";
            }
        }
        std::cout << endl;
    }
}

// void Utilities::GridFull::percolate(Node* source){
    
// }


