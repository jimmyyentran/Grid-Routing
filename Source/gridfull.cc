#include "../Headers/gridfull.h"
#include "../Headers/claim.h"
#include <algorithm> // find vector
#include <iostream> // testing
#include <time.h> // sleep for testing

/*
    NODE-TYPE CONSTANTS:
    const int kSink = 1;
    const int kSource = 0;
    const int kFree = -1;
    const int kBorder = -2;
    const int kVisited = -3;
    const int kBlock = -4;
    const int kPath = -5;
*/

const int kSink = 1;
const int kSource = 0;
const int kFree = -1;
const int kBorder = -2;
const int kVisited = -3;
const int kBlock = -4;
const int kPath = -5;

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
        if(current_sink->get_status() == kBlock){
            claim("Connection " + name + " sink is in a blocker", kError);
            return;
        }
        
        //Make sure sinks and sources aren't the same
        if(source_y == sink_y && source_x == sink_x){
            claim("Connection " + name + " source and sink are the same coordinates", kError);
            return;
        }
        
        // Insert connection's source and sink nodes into queue
        connections.push(std::pair<NodeFull*, NodeFull*> (current_source, current_sink));
    }
}

Utilities::NodeFull* Utilities::GridFull::load_connection(){
    if(connections.empty()){
        claim("Empty connection vector", kError);
        return 0;
    }
    connections.front().first->set_status(kSource);
    connections.front().first->set_cost(0);
    connections.front().second->set_status(kSink);
    connections.front().second->set_cost(0);
    return connections.front().first;
}

void Utilities::GridFull::print_grid(){
    int width = get_width();    // decrease call cost
    for(int y = get_height() - 1; y >= 0; --y){
        for(int x = 0; x < width; ++x){
            int status = grid.at(y).at(x)->get_status();
            switch(status){
                case kFree:
                    // the current_node is free/empty;
                    std::cout << ' ';
                    break;
                case kBlock:
                    // the current_node is a block
                    std::cout << '~';
                    break;
                case kSource:
                    std::cout << '@';
                    break;
                case kSink:
                    std::cout << '&';
                    break;
                case kVisited:
                    std::cout << grid.at(y).at(x)->get_cost() % 10;
                    break;
                case kBorder:
                    std::cout << 'B';
                    break;
                case kPath:
                    std::cout << '*';
                    break;
            }
        }
        std::cout << endl;
    }
    std::cout << endl;
}

vector<Utilities::Path*> Utilities::GridFull::run_lee(Type type){
    while(!connections.empty()){
        run_lee_step(type);
    }
    // Test
    print_grid();
    
    return paths;
}

void Utilities::GridFull::run_lee_step(Type type){
    // input the first node (source) into queue
    claim("Load connection", kNote);
    NodeFull* source_node = load_connection();
    if(source_node == 0){
        return;
    }
    border.push(source_node);
    bool path_found = false;
    
    claim("Begin Search", kNote);
    while(!border.empty()){
        NodeFull* front = border.front();
        // start searching
        if(search_north(front, type) || search_east(front, type) || search_south(front, type)
            || search_west(front, type)){
                path_found = true;
                claim("Found Path!", kNote);
                break;    
        }
        border.front()->set_status(kVisited);
        border.pop();
    }
    
    // reset source to the source node
    claim("Reset source's status", kNote);
    source_node->set_status(kSource);
    
    // Re-Trace path
    if(!path_found){
        claim("No path found", kWarning);
        print_grid();
        
        // convert source and sink into paths
        claim("Converting to kPath", kNote);
        source_node->set_status(kFree);
        connections.front().second->set_status(kFree);
    } else{
        claim("Begin tracing", kNote);
        backtrack();
        //TEST
        print_grid();
        
        // convert source and sink into paths
        claim("Converting to kPath", kNote);
        source_node->set_status(kPath);
        connections.front().second->set_status(kPath);
        
        // make stack into vector of paths
        claim("Adding to vector or Paths", kNote);
        convert_to_path();
    }
    // all border and visited nodes cleared status and sinks
    claim("Cleaning up grid", kNote);
    clean_up_grid();
    
    while(!border.empty()){
        border.pop();
    }
    
    // pop the finished connection
    claim("Pop connection", kNote);
    connections.pop();
    
    claim("Finished connection!\n", kNote);
}



void Utilities::GridFull::clean_up_grid(){
    int width = get_width();    // decrease call cost
    for(int y = get_height() - 1; y >= 0; --y){
        for(int x = 0; x < width; ++x){
            NodeFull* current_node = grid.at(y).at(x);
            int status = current_node->get_status();
            if(status == kBorder || status == kVisited){
                current_node->set_status(kFree);
                current_node->set_cost(0);
            }
        }
    }
}

void Utilities::GridFull::convert_to_path(){
    Path* new_path = new Path();
    while(!trace_path.empty()){
        int x = trace_path.top()->get_x();
        int y = trace_path.top()->get_y();
        Point head(x, y);
        trace_path.pop();
        if(trace_path.empty()){
            break;
        }
        x = trace_path.top()->get_x();
        y = trace_path.top()->get_y();
        Point tail(x, y);
        PathSegment* new_segment = new PathSegment(head, tail);
        new_path->add_segment(new_segment);
    }
    paths.push_back(new_path);
}

void Utilities::GridFull::print_path(){
	for(unsigned i = 0; i < paths.size(); i++) {
    	std::cout << "Path " << i << ": ";
    	paths.at(i)->print();
    	std::cout << endl;
    }
}

void Utilities::GridFull::backtrack(){
    while(trace_path.top()->get_status() != kSource){
        if(backtrack_north() || backtrack_east() || backtrack_south() || backtrack_west()){
            return;
        }
    }
}


bool Utilities::GridFull::backtrack_north(){
    int current_y = trace_path.top()->get_y() - 1;
    int current_x = trace_path.top()->get_x();
    if(current_y >= 0 && grid.at(current_y).at(current_x)->get_cost() + 1 == trace_path.top()->get_cost()){
        NodeFull* current = grid.at(current_y).at(current_x);
        if(current->get_status() == kSource){
            trace_path.push(current);
            return true;
        } else{
            trace_path.push(current);
            current->set_status(kPath);
            backtrack_north();
        }
    }
    return false;
}

bool Utilities::GridFull::backtrack_east(){
    int current_y = trace_path.top()->get_y();
    int current_x = trace_path.top()->get_x() - 1;
    if(current_x >= 0 && grid.at(current_y).at(current_x)->get_cost() + 1 == trace_path.top()->get_cost()){
        NodeFull* current = grid.at(current_y).at(current_x);
        if(current->get_status() == kSource){
            trace_path.push(current);
            return true;
        } else{
            trace_path.push(current);
            current->set_status(kPath);
            backtrack_east();
        }
    }
    return false;
}

bool Utilities::GridFull::backtrack_south(){
    int current_y = trace_path.top()->get_y() + 1;
    int current_x = trace_path.top()->get_x();
    if(current_y < get_height() && grid.at(current_y).at(current_x)->get_cost() + 1 == trace_path.top()->get_cost()){
        NodeFull* current = grid.at(current_y).at(current_x);
        if(current->get_status() == kSource){
            trace_path.push(current);
            return true;
        } else{
            trace_path.push(current);
            current->set_status(kPath);
            backtrack_south();
        }
    }
    return false;
}

bool Utilities::GridFull::backtrack_west(){
    int current_y = trace_path.top()->get_y();
    int current_x = trace_path.top()->get_x() + 1;
    if(current_x < get_width() && grid.at(current_y).at(current_x)->get_cost() + 1 == trace_path.top()->get_cost()){
        NodeFull* current = grid.at(current_y).at(current_x);
        if(current->get_status() == kSource){
            trace_path.push(current);
            return true;
        } else{
            trace_path.push(current);
            current->set_status(kPath);
            backtrack_west();
        }
    }
    return false;
}

// void Utilities::GridFull::insert_node_path(NodeFull* node, Direction direction){
//     trace_path.push(std::pair<NodeFull*, Direction>(node, direction));
// }

bool Utilities::GridFull::search_north(NodeFull* border_node, Type type){
    int north_y = border_node->get_y() + 1;
    int north_x = border_node->get_x();
    int cost = border_node->get_cost() + 1;
    if(north_y < get_height()){
        // node is in grid range
        return increment_path(grid.at(north_y).at(north_x), cost, type);
        // if(increment_path(grid.at(north_y).at(north_x),cost)){
        //     // insert_node_path(border_node, kNorth);
        //     return true;
        // }
    }
    return false;
}

bool Utilities::GridFull::search_west(NodeFull* border_node, Type type){
    int west_y = border_node->get_y();
    int west_x = border_node->get_x() - 1;
    int cost = border_node->get_cost() + 1;
    if(west_x >= 0){
        // Western node is within grid limits
        return increment_path(grid.at(west_y).at(west_x), cost, type);
        // if(increment_path(grid.at(west_y).at(west_x),cost)){
        //     // insert_node_path(border_node, kWest);
        //     return true;
        // }
    }
    return false;
}

bool Utilities::GridFull::search_south(NodeFull* border_node, Type type){
    int south_y = border_node->get_y() - 1;
    int south_x = border_node->get_x();
    int cost = border_node->get_cost() + 1;
    if(south_y >= 0){
        // Southern node is within grid 
        return increment_path(grid.at(south_y).at(south_x), cost, type);
        // if(increment_path(grid.at(south_y).at(south_x),cost)){
        // //   insert_node_path(border_node, kSouth);
        //     return true;
        // }
    }
    return false;
}

bool Utilities::GridFull::search_east(NodeFull* border_node, Type type){
    int east_y = border_node->get_y();
    int east_x = border_node->get_x() + 1;
    int cost = border_node->get_cost() + 1;
    if(east_x < get_width()){
        // Eastern node is within grid limits
        return increment_path(grid.at(east_y).at(east_x), cost, type);
        // if(type == kNormal){
        //     return increment_path(grid.at(east_y).at(east_x), cost, type);
        // } else if(type == k3bit){
        //     return increment_path(grid.at(east_y).at(east_x), cost, type);
        // }
        // if(increment_path(grid.at(east_y).at(east_x),cost)){
        //     // insert_node_path(border_node, kEast);
        //     return true;
        // }
    }
    return false;
}

bool Utilities::GridFull::increment_path(NodeFull* new_border_node, int cost, Type type){
    if(type == k3bit){
        cost = (cost % 3) + 1;
    }
    if(new_border_node->get_status() == kSink){
        new_border_node->set_cost(cost);
        trace_path.push(new_border_node);
        return true;
    }
    if(new_border_node->get_status() >= kFree){
        new_border_node->set_status(kBorder);
        new_border_node->set_cost(cost);
        border.push(new_border_node);
    }
    return false;
}

// bool Utilities::GridFull::increment_path_3bit(NodeFull* new_border_node, int cost){
//     int cost_3bit = (cost % 3) + 1;
//     if(new_border_node->get_status() == kSink){
//         new_border_node->set_cost(cost_3bit);
//         trace_path.push(new_border_node);
//         return true;
//     }
//     if(new_border_node->get_status() >= kFree){
//         new_border_node->set_status(kBorder);
//         new_border_node->set_cost(cost_3bit);
//         border.push(new_border_node);
//     }
//     return false;
// }

// bool Utilities::GridFull::search_north_3bit(NodeFull* border_node){
//     int north_y = border_node->get_y() + 1;
//     int north_x = border_node->get_x();
//     int cost = border_node->get_cost() + 1;
//     if(north_y < get_height()){
//         // node is in grid range
//         if(increment_path_3bit(grid.at(north_y).at(north_x),cost)){
//             // insert_node_path(border_node, kNorth);
//             return true;
//         }
//     }
//     return false;
// }

// bool Utilities::GridFull::search_west_3bit(NodeFull* border_node){
//     int west_y = border_node->get_y();
//     int west_x = border_node->get_x() - 1;
//     int cost = border_node->get_cost() + 1;
//     if(west_x >= 0){
//         // Western node is within grid limits
//         if(increment_path_3bit(grid.at(west_y).at(west_x),cost)){
//             // insert_node_path(border_node, kWest);
//             return true;
//         }
//     }
//     return false;
// }

// bool Utilities::GridFull::search_south_3bit(NodeFull* border_node){
//     int south_y = border_node->get_y() - 1;
//     int south_x = border_node->get_x();
//     int cost = border_node->get_cost() + 1;
//     if(south_y >= 0){
//         // Southern node is within grid limits
//         if(increment_path_3bit(grid.at(south_y).at(south_x),cost)){
//         //   insert_node_path(border_node, kSouth);
//             return true;
//         }
//     }
//     return false;
// }

// bool Utilities::GridFull::search_east_3bit(NodeFull* border_node){
//     int east_y = border_node->get_y();
//     int east_x = border_node->get_x() + 1;
//     int cost = border_node->get_cost() + 1;
//     if(east_x < get_width()){
//         // Eastern node is within grid limits
//         if(increment_path_3bit(grid.at(east_y).at(east_x),cost)){
//             // insert_node_path(border_node, kEast);
//             return true;
//         };
//     }
//     return false;
// }

// void Utilities::GridFull::run_lee_step_3bit(){
//     // input the first node (source) into queue
//     NodeFull* source_node = load_connection();
//     if(source_node == 0){
//         return;
//     }
//     border.push(source_node);
    
//     bool path_found = false;
//     while(!border.empty()){
//         NodeFull* front = border.front();
//         // start searching
//         if(search_north_3bit(front) || search_east_3bit(front) || search_south_3bit(front)
//             || search_west_3bit(front)){
//                 path_found = true;
//                 break;    
//         }
//         border.front()->set_status(kVisited);
//         border.pop();
//     }
    
//     // reset source to the source node
//     source_node->set_status(kSource);
    
//     print_grid();
//     // // Re-Trace path
//     // if(!path_found){
//     //     claim("No path found", kWarning);
//     // } else{
//     //     backtrack();
    
//     //     //TEST
//     //     print_grid();
        
//     //     // convert source and sink into paths
//     //     source_node->set_status(kPath);
//     //     connections.front().second->set_status(kPath);
        
//     //     // make stack into vector of paths
//     //     convert_to_path();
//     // }
    
//     // // all border and visited nodes cleared status and sinks
//     // clean_up_grid();
    
//     // while(!border.empty()){
//     //     border.pop();
//     // }
    
//     // // pop the finished connection
//     // connections.pop();
// }

// void Utilities::GridFull::percolate(Node* source){
    
// }


