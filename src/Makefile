## Example invocations. If you change between debug and release, don't forget to make clean
#make release OPT=1
#make debug DBG=1 OPT=1

## Variables from command line
OPT=0 #-DOPTIMISTIC
DBG=0 #-DDEBUG
VERB=0 #-DVERBOSE
AN=0 #-DENABLE_ANALYSIS


## Compiler and standard flags
#COMPILER = g++ # Linux
COMPILER = icpc # XeonPhi
#FLAGS = -std=c++11 -fopenmp # Linux
FLAGS = -mmic -fopenmp -std=c++11 # XeonPhi


ALGORITHMS = serial omp_tasks omp_locallist omp_bitset #omp_basic  # --> serial
EXECUTABLES = $(addprefix toposort_, $(addsuffix .exe, $(ALGORITHMS))) # --> toposort_serial.exe
OBJECTS = $(addprefix graphsort_, $(addsuffix .o, $(ALGORITHMS))) # --> graphsort_serial.o

GRAPHSRC_DIR := graph_output
GRAPHSRC_FILES := $(wildcard $(GRAPHSRC_DIR)/*.gv)
GRAPHIMG_FILES := $(GRAPHSRC_FILES:.gv=.png)

# Attention: this messes with flags that are set above. Use with care, i.e. make clean first
debug: FLAGS += -g -O0
debug: DBG=2
debug: VERB=1
debug: all

relwithdebinfo: FLAGS+= -g
relwithdebinfo: release

release: FLAGS += -O3
release: DBG=0
release: VERB=0
release: all

all: FLAGS += -DVERBOSE=$(VERB) -DDEBUG=$(DBG) -DOPTIMISTIC=$(OPT) -DENABLE_ANALYSIS=$(AN)
all: $(EXECUTABLES)

$(EXECUTABLES): toposort_%.exe: graphsort_%.o main_toposort.o graph.o graphdoc.o node.o analysis.o
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

analysis.o: analysis.cpp analysis.hpp
	$(COMPILER) $(FLAGS) -c analysis.cpp

run: all
	./toposort_omp_locallist.exe

viz: $(GRAPHIMG_FILES)
	display $(GRAPHIMG_FILES);

%.png: %.gv
	# circo -Tpng $< -o $@
	dot -Tpng $< -o $@

clean:
	rm -rf $(EXECUTABLES) *.o
