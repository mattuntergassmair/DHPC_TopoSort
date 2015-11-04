#include <iostream>

#include "graph.hpp"


int main() {

	
	// Initializing graph
	DirGraph testgraph_paper(9);
	testgraph_paper.connect(DirGraph::PAPER); // Constructing graph from paper
	testgraph_paper.topSort();
	testgraph_paper.checkCorrect();
	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");
	
	// Initializing graph with random edges
	unsigned const N = 20;
	DirGraph testgraph_random(N);
	testgraph_random.connect(DirGraph::RANDOM_EDGES); // Constructing graph from paper
	testgraph_random.topSort();
	testgraph_random.checkCorrect();
	testgraph_random.printNodeInfo();
	testgraph_random.viz("random20");

	return 0;

}
