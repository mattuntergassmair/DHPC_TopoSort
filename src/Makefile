COMPILER = g++
FLAGS = -std=c++11 -O0

GRAPH_DIR := graph_output
GRAPHSRC_FILES := $(wildcard $(GRAPH_DIR)/*.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)


all: toposort.exe

toposort.exe: main_toposort.o graph.o graphdoc.o node.o
	$(COMPILER) $(FLAGS) main_toposort.o graph.o graphdoc.o node.o -o toposort.exe

main_toposort.o: main_toposort.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c main_toposort.cpp

graph.o: graph.cpp graph.hpp
	$(COMPILER) $(FLAGS) -c graph.cpp

graphdoc.o: graphdoc.cpp graph.hpp
	$(COMPILER) $(FLAGS) -c graphdoc.cpp

node.o: node.cpp node.hpp
	$(COMPILER) $(FLAGS) -c node.cpp

run: toposort.exe
	./toposort.exe

viz: $(GRAPHIMG_FILES)
	display $(GRAPH_DIR)/*.png;

%.png: %.gv
	# circo -Tpng $< -o $@
	dot -Tpng $< -o $@


clean:
	rm -rf *.exe *.o