#ifndef _GRID_FULL_H_
#define _GRID_FULL_H_

#include "nodefull.h"
#include "path.h"
#include "problem_object.h"
#include <vector>
#include <queue>
#include <stack>
// #include <utility>

namespace Utilities {
    class GridFull{
        private:
            vector<vector<NodeFull*> > grid;   
            std::queue<std::pair<NodeFull*, NodeFull*> > connections;
            std::queue<NodeFull*> border;
            std::stack<NodeFull*> trace_path;
            vector<Path*> paths;
            
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
            vector<Path*> run_lee();
            void run_lee_step();
            NodeFull* load_connection();
            bool increment_path(NodeFull*, int);
            void convert_to_path();
            void backtrack();
            void clean_up_grid();
            
            bool backtrack_north();
            bool backtrack_south();
            bool backtrack_west();
            bool backtrack_east();
            bool search_north(NodeFull*);
            bool search_west(NodeFull*);
            bool search_south(NodeFull*);
            bool search_east(NodeFull*);
            
            // testers
            void print_grid();
            void print_path();
    };
}


#endif