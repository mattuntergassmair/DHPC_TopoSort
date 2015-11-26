COMPILER = g++ # Linux
# COMPILER = icc # XeonPhi
FLAGS = -O0 -std=c++11 -fopenmp -D VERBOSE=1 -D OPTIMISTIC=1 # Linux
# FLAGS = -mmic -fopenmp

GENERICDEPENDENCIES = main_toposort.o graph.o graphdoc.o node.o # TODO: fix this

GRAPHSRC_FILES := $(wildcard *.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)


OBJECTS =  graphsort_serial.o graphsort_omp_basic.o graphsort_omp_tasks.o graphsort_omp_locallist.o graphsort_omp_bitset.o 


all: toposort_omp_locallist.exe
	# toposort_serial.exe toposort_omp_basic.exe toposort_omp_tasks.exe 


toposort_serial.exe: graphsort_serial.o $(GENERICDEPENDENCIES) 
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_basic.exe: graphsort_omp_basic.o $(GENERICDEPENDENCIES)
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_tasks.exe: graphsort_omp_tasks.o $(GENERICDEPENDENCIES)
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_locallist.exe: graphsort_omp_locallist.o $(GENERICDEPENDENCIES)
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_bitset.exe: graphsort_omp_bitset.o $(GENERICDEPENDENCIES)
	$(COMPILER) $(FLAGS) $^ -o $@


main_toposort.o: main_toposort.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $< 

graph.o: graph.cpp graph.hpp node.hpp analysis.hpp
	$(COMPILER) $(FLAGS) -c $<


$(OBJECTS): %.o: %.cpp graph.hpp node.hpp analysis.hpp
	$(COMPILER) $(FLAGS) -c $<


graphdoc.o: graphdoc.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c graphdoc.cpp

node.o: node.cpp node.hpp
	$(COMPILER) $(FLAGS) -c node.cpp


run: toposort_omp_basic.exe
	./toposort_omp_basic.exe

viz: $(GRAPHIMG_FILES)
	display *.png;

%.png: %.gv
	# circo -Tpng $< -o $@
	dot -Tpng $< -o $@

clean:
	rm -rf *.exe *.o
