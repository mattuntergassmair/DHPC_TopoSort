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
		// TODO: memory leak here - using smart pointers? 

		DirGraph(unsigned N)
			: N_(N)
			, nodes_(nodearray_type(N_))
		{
			std::cout << "Initialized graph of size " << N_ << "\n";
			for(unsigned i=0; i<N_; ++i) {
				// TODO: memory leak here - using smart pointers? 
				nodes_[i] = std::make_shared<Node>(i);
			}
		}

		void printNodeInfo();

		void topSort();
		void topSortParallel();
		void connect(unsigned, double edgeFillDegree = .3);
        bool checkCorrect();
		void viz(std::string) const;
		void reset(); // resets the graph so it can be sorted again

	private:

		unsigned N_;
		nodearray_type nodes_;

};


#endif // DIRGRAPH_HPP
