COMPILER = g++
FLAGS = -O0


all: toposort.exe

toposort.exe: main_toposort.o graph.o node.o
	$(COMPILER) $(FLAGS) main_toposort.o graph.o node.o -o toposort.exe

main_toposort.o: main_toposort.cpp graph.hpp node.hpp
	$(COMPILER) $(FLAGS) -c main_toposort.cpp

graph.o: graph.cpp graph.hpp
	$(COMPILER) $(FLAGS) -c graph.cpp

node.o: node.cpp node.hpp
	$(COMPILER) $(FLAGS) -c node.cpp

run: toposort.exe
	./toposort.exe

clean:
	rm -rf *.exe *.o
