#include "graph.hpp"

#include <cassert>
#include <string>
#include <cstdio>
#include <list>

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
	std::string path = std::string(CMAKE_SOURCE_DIR) + std::string(DS) + "graph_output" + std::string(DS) + graphfilename;
	outfile_ptr = fopen(path.c_str(),"w");
	if(outfile_ptr != NULL){
		fprintf(outfile_ptr,"# Visualization of Graph %s, size=%u\n\n",path.c_str(),N_);
		fprintf(outfile_ptr,"digraph g {\n");
		// fprintf(outfile_ptr,"\tranksep=1;\n");
		// fprintf(outfile_ptr,"\tratio=auto;\n");
		
		fprintf(outfile_ptr,"\n\t#TITLE\n\tlabelloc=\"t\";\n\tlabel=\"type=%s, size=%u;\"\n",path.c_str(),N_);
		
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
		std::cout << "\n\nGraph file created as " << graphfilename << std::endl;
	}
	else{
		std::cerr << "\n\nPath doesn't exist: " << path << std::endl;
	}
}
