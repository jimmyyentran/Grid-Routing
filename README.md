Routing Project Framework
===============

Implementation of Lee's Algorithm and Hadlock's Algorithm using the original project's source code located at: 
https://github.com/brrcrites/Routing_Framework

Analysis:

I went through 3 different type of data storage: sets, maps, and vectors. Initially I used sets
to insert all blocks, sources, and sink nodes into individual containers. I though sets would be a good idea
due to ever element's uniqueness. This idea was flawed because to find whether a node was a block, 
source, or sink, I would have to traverse each set and therefore wasting computing time. 

Secondly I used the STL map to store all special nodes (blocks, sources, or sinks) into the key being the Node*
and the vaule being the type of the node. Storing all node's type under one map uses less memory and is a 
more readable code. However, I realize that the lookup time is still quite long O(log n) in comparison to
vectors.

Finally I settled on vectors due to O(1) search time. I implemented a grid class (GridFull) that used similar
construction as the provided Grid class. I also derived a child class from the Node class (NodeFull) that
most importantly contain a 'status' data member that holds the node's type. I figure that I can find adjacent
nodes by using the x and y coordinate so I did not utilize edges(yet) in the grid construction.

7-15-2015
2bit Lee's algorithm uses a queue that is sectioned off by a null-node. This is different than the previous
Lee's algorithm.
