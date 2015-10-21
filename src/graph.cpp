#include "graph.hpp"
#include <cassert>

void DirGraph::topSort() {
	// Sorting Magic happens here
	// TODO: Johannes
	
}

void DirGraph::connect(unsigned type) {
	
	std::cout << "\nConnection Mode:\t";

	Node nd = Node(2);

	switch(type) {

		case PAPER: // Construct simple example graph from paper
			assert(N_==9);
			nodes_[0]->addChild(*nodes_[2]);
			nodes_[2]->addChild(*nodes_[6]);
			nodes_[6]->addChild(*nodes_[3]);
			nodes_[6]->addChild(*nodes_[4]);
			nodes_[3]->addChild(*nodes_[5]);
			nodes_[4]->addChild(*nodes_[7]);
			nodes_[7]->addChild(*nodes_[5]);
			nodes_[1]->addChild(*nodes_[7]);
			nodes_[8]->addChild(*nodes_[1]);
			nodes_[8]->addChild(*nodes_[4]);
			std::cout << "Paper";
			break;

		default:
			std::cout << "\nERROR:\tInvalid connection index - no connections added\n";
			
	}

	std::cout << "\n";

}

void DirGraph::printNodeInfo() {
	for(unsigned i=0; i<N_; ++i) {
		std::cout << "ID: " << nodes_[i]->getID()
				<< "\tV: " << nodes_[i]->getValue() << "\n";
	}
}
