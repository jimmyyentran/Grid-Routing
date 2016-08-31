
SRC=$(filter-out %main.cc, Source/*)
OBJ=claim.o edge.o grid.o netlist.o node.o path.o pathsegment.o problem_object.o segmentgroup.o gridfull.o nodefull.o
CODE_DIR = Utilities/JSON_parser/

vpath %.cc Source/

all: $(OBJ) main.cc
	$(MAKE) -C $(CODE_DIR)
	g++ -o grid_router $^ Utilities/JSON_parser/json_parser.so 

test: all
	./grid_router Tests/test_sample.json
	
%.o: %.cc
	g++ -c $^

cleanup:
	rm -f *.o

clean: cleanup
	rm -f grid_router

