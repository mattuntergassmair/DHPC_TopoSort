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

// Print Node Info to console
void DirGraph::printNodeInfo() {
	for(unsigned i=0; i<N_; ++i) {
		std::cout << "ID: " << nodes_[i]->getID()
				<< "\tV: " << nodes_[i]->getValue() << "\n";
	}
}

// Create graphviz file for drawing graph
void DirGraph::viz(std::string graphfilename) const {
	graphfilename.append(".gv");

	FILE* outfile_ptr;
	outfile_ptr = fopen(graphfilename.c_str(),"w");
	fprintf(outfile_ptr,"# Visualization of Graph %s, size=%u\n\n",graphfilename.c_str(),N_);
	fprintf(outfile_ptr,"digraph g {\n");
	// fprintf(outfile_ptr,"\tranksep=1;\n");
	// fprintf(outfile_ptr,"\tratio=auto;\n");
	
	fprintf(outfile_ptr,"\n\t#TITLE\n\tlabelloc=\"t\";\n\tlabel=\"type=%s, size=%u;\"\n",graphfilename.c_str(),N_);
	
	fprintf(outfile_ptr,"\n\t# NODES\n");
	for(unsigned n=0; n<N_; ++n) {
		fprintf(outfile_ptr,"\tN%02u [ color=\"#000000\", fillcolor=\"%s\", shape=\"circle\", style=\"filled,solid\" ];\n",n,"#66ffcc"); // TODO: calculate color based on value
	}

	fprintf(outfile_ptr,"\n\t# EDGES\n");
	unsigned cc;
	for(unsigned n=0; n<N_; ++n) {
		cc = nodes_[n]->getChildCount(); // number of children of current node
		for(unsigned c=0; c<cc; ++c) { // draw arrow to all children
			fprintf(outfile_ptr,"\tN%02u -> N%02u [ penwidth=2, style=\"solid\", color=\"#000000\" ];\n",n,(nodes_[n]->getChild(c))->getID());
		}
	}
	
	fprintf(outfile_ptr,"}");

	fclose(outfile_ptr);
	std::cout << "\n\nGraph file created as " << graphfilename;

}
