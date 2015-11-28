#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "node.hpp"
#include "analysis.hpp"


class Graph {

	public:

		enum GRAPH_TYPE {PAPER, RANDOM_EDGES};

		using type_nodeptr = std::shared_ptr<Node>;
		using type_nodearray = std::vector<type_nodeptr>;
        using type_nodelist = std::list<type_nodeptr>;
        using type_solution = type_nodelist; // NB: I would prefer type_nodelist over type_solution - more generic (not all nodelists are solutions, for example currentnodes)
        using type_size = analysis::type_size;
        
        explicit Graph(unsigned N)
			:	N_(N)
			,	nodes_(type_nodearray(N_))
			,	A_()
		{
			std::cout << "Initializing graph of size " << N_ << "...\n";
			for(unsigned i=0; i<N_; ++i) {
				nodes_[i] = std::make_shared<Node>(i);
			}
		}
		analysis::type_time time_topSort() {
			A_.starttotaltiming();
			this->topSort();
			A_.stoptotaltiming();
			std::cout << "\nSorting completed in:\t" << A_.time_Total_ << " sec\n\n";
			return A_.time_Total_;
		}
        void topSort();
        
		void connect(unsigned, double edgeFillDegree = .3);
        bool checkCorrect(bool verbose);
        type_solution getSolution();
        
        // Print and doc methods (graphdoc.cpp)
		void printNodeInfo();
        void printSolution();
		void viz(std::string) const;
        
	protected:

		unsigned N_;
		type_nodearray nodes_;
        type_solution solution_;
        analysis A_;

};


#endif // GRAPH_HPP
