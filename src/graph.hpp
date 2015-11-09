#ifndef DIRGRAPH_HPP
#define DIRGRAPH_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "node.hpp"

class DirGraph {

	public:

		enum GRAPH_TYPE {PAPER, RANDOM_EDGES};

		typedef std::vector<std::shared_ptr<Node> > nodearray_type;
        typedef std::list<std::shared_ptr<Node> > solution_type;
		DirGraph(unsigned N)
			: N_(N)
			, nodes_(nodearray_type(N_))
		{
			std::cout << "Initialized graph of size " << N_ << "\n";
			for(unsigned i=0; i<N_; ++i) {
				nodes_[i] = std::make_shared<Node>(i);
			}
		}

		void topSort();
		void connect(unsigned, double edgeFillDegree = .3);
        bool checkCorrect();
        solution_type getSolution();
        
        // Print and doc methods (graphdoc.cpp)
		void printNodeInfo();
        void printSolution();
		void viz(std::string) const;
        
	private:

		unsigned N_;
		nodearray_type nodes_;
        solution_type solution_;

};


#endif // DIRGRAPH_HPP
