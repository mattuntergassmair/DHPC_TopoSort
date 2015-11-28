#include <iostream>
#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"


int main() {
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
	testgraph_paper.topSort();
	testgraph_paper.checkCorrect(true);
	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");
    testgraph_paper.printSolution();


	// RANDOM GRAPH
	unsigned const N = 5000;
	Graph testgraph_random(N);
	testgraph_random.connect(Graph::RANDOM_EDGES, 0.05);
	testgraph_random.topSort();

    // double t_total = t.sec();
	testgraph_random.checkCorrect(false);
	
	// Initializing graph with random edges, with analysis
	// GraphAnalysis testgraph_random2(N);
	// testgraph_random2.connect(Graph::RANDOM_EDGES, 0.05);
    
    // // t.start();
	// testgraph_random2.topSort();
    // // t.stop();
	// testgraph_random2.checkCorrect(false);
	// TODO: not necessary - just make separate runs and write to database 
    // testgraph_random2.analysis_.t_total = t_total;
    // testgraph_random2.analysis_.t_timing_overhead = t.sec() - t_total;
    // testgraph_random2.analysis_.summary(std::cout);
	return 0;

}
