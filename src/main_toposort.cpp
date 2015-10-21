#include <iostream>

#include "graph.hpp"


int main() {

	// Initializing graph
	DirGraph testgraph_paper(9);
	testgraph_paper.connect(DirGraph::PAPER); // Constructing graph from paper
	testgraph_paper.topSort();

	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");

	return 0;

}
