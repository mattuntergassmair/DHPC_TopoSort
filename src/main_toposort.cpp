#include <iostream>

#include "graph.hpp"
#include "Timer.hpp"


int main() {

	Timer t;

	// Initializing graph
	DirGraph testgraph_paper(9);
	testgraph_paper.connect(DirGraph::PAPER); // Constructing graph from paper
	testgraph_paper.topSort();
	testgraph_paper.checkCorrect();
	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");
	
#pragma omp parallel
printf("asd");

	// Initializing graph with random edges
	unsigned const N = 3000;
	DirGraph testgraph_random(N);
	testgraph_random.connect(DirGraph::RANDOM_EDGES, 0.05); // Constructing graph from paper

	t.start();
	testgraph_random.topSort();
	t.stop();
	

	testgraph_random.checkCorrect();
	t.printElapsed();
	//testgraph_random.printNodeInfo();
	//testgraph_random.viz("random20");

	testgraph_random.reset();

	t.start();
	testgraph_random.topSortParallel();
	t.stop();

	testgraph_random.checkCorrect();
	t.printElapsed();





	return 0;

}
