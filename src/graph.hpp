#ifndef DIRGRAPH_HPP
#define DIRGRAPH_HPP

#include <iostream>
#include <vector>
#include <algorithm>

#include "node.hpp"

class DirGraph {

	public:

		enum GRAPH_TYPE {PAPER};

		typedef std::vector<Node*> nodearray_type; // TODO: what is the best data structure here?

		DirGraph(unsigned N)
			: N_(N)
			, nodes_(nodearray_type(N_))
		{
			std::cout << "Initialized graph of size " << N_ << "\n";
			for(unsigned i=0; i<N_; ++i) {
				nodes_[i] = new Node(i);
			}
		}

		void printNodeInfo();

		void topSort();
		void connect(unsigned);

		inline Node* getNextNode(); // TODO: Kevin


	private:

		unsigned N_;
		nodearray_type nodes_;

};


#endif // DIRGRAPH_HPP
