COMPILER = g++ # Linux
# COMPILER = icc # XeonPhi
FLAGS = -O0 -std=c++11 -fopenmp -D VERBOSE=1 -D OPTIMISTIC=1 # Linux
# FLAGS = -mmic -fopenmp

GENERICDEPENDENCIES = graph.o graphdoc.o node.o # TODO: fix this

GRAPHSRC_FILES := $(wildcard *.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)


all: prepare toposort_omp_locallist.exe
	# toposort_serial.exe toposort_omp_basic.exe toposort_omp_tasks.exe 

prepare:
	cp graph.hpp graph.analysis.hpp

toposort_serial.exe: graph.o graphsort_serial.o graphdoc.o graph.analysis.o graphsort_serial.analysis.o graphdoc.analysis.o node.o main_toposort.o 
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_basic.exe: graph.o graphsort_omp_basic.o graphdoc.o graph.analysis.o graphsort_omp_basic.analysis.o graphdoc.analysis.o node.o main_toposort.o
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_locallist.exe: graph.o graphsort_omp_locallist.o graphdoc.o graph.analysis.o graphsort_omp_locallist.analysis.o graphdoc.analysis.o node.o main_toposort.o
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_tasks.exe: graph.o graphsort_omp_tasks.o graphdoc.o graph.analysis.o graphsort_omp_tasks.analysis.o graphdoc.analysis.o node.o main_toposort.o
	$(COMPILER) $(FLAGS) $^ -o $@

toposort_omp_bitset.exe: graph.o graphsort_omp_bitset.o graphdoc.o graph.analysis.o graphsort_omp_bitset.analysis.o graphdoc.analysis.o node.o main_toposort.o
	$(COMPILER) $(FLAGS) $^ -o $@

main_toposort.o: main_toposort.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $< 

graph.o: graph.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graph.analysis.o: graph.cpp graph.analysis.hpp node.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graph.analysis.o $<

graphsort_serial.o: graphsort_serial.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_serial.analysis.o: graphsort_serial.cpp graph.analysis.hpp node.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graphsort_serial.analysis.o $<

graphsort_omp_basic.o: graphsort_omp_basic.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_basic.analysis.o: graphsort_omp_basic.cpp graph.analysis.hpp node.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graphsort_omp_basic.analysis.o $<

graphsort_omp_locallist.o: graphsort_omp_locallist.cpp graph.hpp node.hpp timerwrapper.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_locallist.analysis.o: graphsort_omp_locallist.cpp graph.analysis.hpp node.hpp timerwrapper.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graphsort_omp_locallist.analysis.o $<

graphsort_omp_tasks.o: graphsort_omp_tasks.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_tasks.analysis.o: graphsort_omp_tasks.cpp graph.analysis.hpp node.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graphsort_omp_tasks.analysis.o $<

graphsort_omp_bitset.o: graphsort_omp_bitset.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c $<

graphsort_omp_bitset.analysis.o: graphsort_omp_bitset.cpp graph.analysis.hpp node.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graphsort_omp_bitset.analysis.o $<

graphdoc.o: graphdoc.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c graphdoc.cpp

graphdoc.analysis.o: graphdoc.cpp graph.analysis.hpp node.hpp
	$(COMPILER) $(FLAGS) -DENABLE_ANALYSIS -c -o graphdoc.analysis.o graphdoc.cpp

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
	rm -rf *.exe *.o *.analysis.cpp *.analysis.hpp
