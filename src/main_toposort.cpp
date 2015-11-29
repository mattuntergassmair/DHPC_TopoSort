#include <iostream>
#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"


int main() {
	// TODO: this can probably be removed from main function
	// should be in topsort instead
#ifndef NDEBUG
    std::cout << "THIS IS THE DEBUG MODE" << std::endl;

    #pragma omp parallel
    #pragma omp master
    std::cout << "Number of threads: " << omp_get_num_threads() << std::endl;
#endif
	// util::rdtsc_timer t; // TODO: access timing from graph

	// PAPER GRAPH - Testing
	Graph testgraph_paper(9);
	testgraph_paper.connect(Graph::PAPER); // Constructing graph from paper
	testgraph_paper.time_topSort();
	testgraph_paper.checkCorrect(true);
	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");
    testgraph_paper.printSolution();


	// RANDOM GRAPH - SMALL
	Graph testgraph_random(40);
	testgraph_random.connect(Graph::RANDOM_EDGES, 0.05);
	testgraph_random.time_topSort();
	testgraph_random.checkCorrect(false);
	
	// RANDOM GRAPH - MEDIUM
	// unsigned const N = 5000;
	// Graph testgraph_random(N);
	// testgraph_random.connect(Graph::RANDOM_EDGES, 0.05);
	// testgraph_random.time_topSort();
	// testgraph_random.checkCorrect(false);

	return 0;

}
