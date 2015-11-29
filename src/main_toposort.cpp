#include <iostream>
#include <omp.h>
#include <string>

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

	std::string visualbarrier(70,'=');
	visualbarrier = "\n\n\n" + visualbarrier + "\n\n";

	// PAPER GRAPH - Testing
	std::cout << visualbarrier;
	Graph testgraph_paper(9);
	testgraph_paper.connect(Graph::PAPER); // Constructing graph from paper
	testgraph_paper.time_topSort();
	testgraph_paper.checkCorrect(true);
	testgraph_paper.viz("paper");


	// RANDOM GRAPH - SMALL
	std::cout << visualbarrier;
	Graph testgraph_random_small(40);
	testgraph_random_small.connect(Graph::RANDOM_EDGES, 0.05);
	testgraph_random_small.time_topSort();
	testgraph_random_small.checkCorrect(false);
	
	// RANDOM GRAPH - MEDIUM
	std::cout << visualbarrier;
	unsigned const N = 5000;
	Graph testgraph_random(N);
	testgraph_random.connect(Graph::RANDOM_EDGES, 0.05);
	testgraph_random.time_topSort();
	testgraph_random.checkCorrect(false);

	std::cout << visualbarrier;
	return 0;

}
