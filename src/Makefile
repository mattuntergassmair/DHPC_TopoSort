COMPILER = g++ # Linux
# COMPILER = icc # XeonPhi
FLAGS = -O0 -std=c++11 -fopenmp -D DEBUG=0 -D VERBOSE=0 -D OPTIMISTIC=0 # Linux
# FLAGS = -mmic -fopenmp


ALGORITHMS = serial omp_basic omp_tasks omp_locallist omp_bitset # --> serial
EXECUTABLES = $(addprefix toposort_, $(addsuffix .exe, $(ALGORITHMS))) # --> toposort_serial.exe
OBJECTS = $(addprefix graphsort_, $(addsuffix .o, $(ALGORITHMS))) # --> graphsort_serial.o

GRAPHSRC_FILES := $(wildcard *.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)


all: $(EXECUTABLES)


$(EXECUTABLES): toposort_%.exe: graphsort_%.o main_toposort.o graph.o graphdoc.o node.o
	$(COMPILER) $(FLAGS) $^ -o $@


main_toposort.o: main_toposort.cpp graph.hpp node.hpp analysis.hpp
	$(COMPILER) $(FLAGS) -c $< 

graph.o: graph.cpp graph.hpp node.hpp analysis.hpp
	$(COMPILER) $(FLAGS) -c $<


$(OBJECTS): %.o: %.cpp graph.hpp node.hpp analysis.hpp
	$(COMPILER) $(FLAGS) -c $<


graphdoc.o: graphdoc.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c graphdoc.cpp

node.o: node.cpp node.hpp
	$(COMPILER) $(FLAGS) -c node.cpp


run: all
	./toposort_omp_locallist.exe

viz: $(GRAPHIMG_FILES)
	display *.png;

%.png: %.gv
	# circo -Tpng $< -o $@
	dot -Tpng $< -o $@

clean:
	rm -rf $(EXECUTABLES) *.o
