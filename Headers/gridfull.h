#ifndef _GRID_FULL_H_
#define _GRID_FULL_H_

#include "node.h"
#include "path.h"
#include "problem_object.h"
#include "grid.h"
#include <vector>
#include <set>

namespace Utilities {
    class GridFull : public Grid {
        private:
            std::set<Node*> blocked_nodes; //set for quicker lookup-time & unique elements
            vector<Node* > source;
            vector<Node* > sink;
        
        public:
            // Constructor
            GridFull(ProblemObject* problem_object);
            
            // object checker
            void include_connections(ProblemObject* problem_object);
            void include_blockers(ProblemObject* problem_object);
            
            // testers
            void print_grid();
            // void check_object_validity(ProblemObject* problem_object);
            
            // additional grid implementations
            // void include_blockers(ProblemObject* problem_object);
            // void include_connections(ProblemObject* problem_object);
            
    };
}


#endif