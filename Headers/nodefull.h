#ifndef _NODE_FULL_H_
#define _NODE_FULL_H_

#include "node.h"

namespace Utilities {
    class NodeFull : public Node {
        private:
            int status;
            /*
                NODE-TYPE CONSTANTS:
                kFree = -1
                kBlock = -2
                kVistited = -3
                kBorder = -4
                
                All sinks and sources are non-negatives. 
                Sources are even
                Sinks are odds and sources + 1
                
                Ex. Source1 = 0, Sink1 = 1. Source2 = 2, Sink2 = 3
            */
        public:
            // Constructor
            NodeFull(int x, int y, int status = -1); // initial node set to kFree
          
            // Accessors
            int get_status();
            
            // Mutators
            void set_status(int status);
    };
}

#endif