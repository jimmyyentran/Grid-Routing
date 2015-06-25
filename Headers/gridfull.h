#ifndef _GRID_FULL_H_
#define _GRID_FULL_H_

#include "nodefull.h"
#include "path.h"
#include "problem_object.h"
#include <vector>
#include <queue>
// #include <utility>

namespace Utilities {
    class GridFull{
        private:
            vector<vector<NodeFull*> > grid;   
            std::queue<std::pair<NodeFull*, NodeFull*> > connections;
            std::queue<NodeFull*> border;
            
            enum Direction {kNorth, kEast, kSouth, kWest, kDirectionSink};
            std::vector<std::pair<NodeFull*, Direction> > trace_path;
            
        public:
            // Constructor
            GridFull(ProblemObject* problem_object);
            
            // Accessors
            int get_width();
            int get_height();
            
            // object checker
            void check_connections(ProblemObject* problem_object);
            void include_blockers(ProblemObject* problem_object);
            
            // algorithm
            void run_lee(int sink);
            NodeFull* load_connection();
            bool increment_path(NodeFull*, int);
            void insert_node_path(NodeFull*, Direction);
            bool search_north(NodeFull*);
            bool search_west(NodeFull*);
            bool search_south(NodeFull*);
            bool search_east(NodeFull*);
            
            // testers
            void print_grid();
    };
}


#endif