COMPILER = g++ # Linux
# COMPILER = icc # XeonPhi
FLAGS = -O0 -std=c++11 -fopenmp -D VERBOSE=1 -D OPTIMISTIC=1 # Linux
# FLAGS = -mmic -fopenmp

GRAPHSRC_FILES := $(wildcard *.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)


all: toposort_serial.exe toposort_omp_basic.exe toposort_omp_tasks.exe

analysis: $(eval FLAGS += -DENABLE_ANALYSIS) all

toposort_serial.exe: main_toposort.o graph.o graphsort_serial.o graphdoc.o node.o
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_basic.exe: main_toposort.o graph.o graphsort_omp_basic.o graphdoc.o node.o
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_tasks.exe: main_toposort.o graph.o graphsort_omp_tasks.o graphdoc.o node.o
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_bitset.exe: main_toposort.o graph.o graphsort_omp_bitset.o graphdoc.o node.o
	$(COMPILER) $(FLAGS) $^ -o $@

main_toposort.o: main_toposort.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $< 

graph.o: graph.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_serial.o: graphsort_serial.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_basic.o: graphsort_omp_basic.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_tasks.o: graphsort_omp_tasks.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_bitset.o: graphsort_omp_bitset.cpp graph.hpp node.hpp
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
