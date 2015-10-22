#include "graph.hpp"

#include <cassert>
#include <string>
#include <cstdio>
#include <list>

void DirGraph::topSort() {
	
	// Sorting Magic happens here
	std::list<Node*> currentnodes;
	for(unsigned i=0; i<N_; ++i) {
		if(nodes_[i]->getValue() == 1) currentnodes.push_back(nodes_[i]);
	}
	Node* parent;
	Node* child;
	unsigned childcount = 0;
	unsigned currentvalue = 0;
	
	while(!currentnodes.empty()) { // stop when queue is empty
		parent = currentnodes.front();
		currentnodes.pop_front(); // remove current node - already visited
		currentvalue = parent->getValue();
		++currentvalue; // increase value for child nodes
		childcount = parent->getChildCount();
		for(unsigned i=0; i<childcount; ++i) {
			child = parent->getChild(i);
			currentnodes.push_back(child); // add child node at end of queue
			child->setValue(currentvalue); // set value of child node to parentvalue+1
		}
	}
	
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

