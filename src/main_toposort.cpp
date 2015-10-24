#include <iostream>

#include "graph.hpp"


int main() {

	
	// Initializing graph
	DirGraph testgraph_paper(9);
	testgraph_paper.connect(DirGraph::PAPER); // Constructing graph from paper
	testgraph_paper.topSort();

	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");
	
	// Initializin graph with random edges
	DirGraph testgraph_20(20);
	testgraph_20.connect(DirGraph::RANDOM_EDGES); // Constructing graph from paper
	testgraph_20.topSort();

	testgraph_20.printNodeInfo();
	testgraph_20.viz("random20");

	return 0;

}
