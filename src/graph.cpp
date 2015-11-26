#include "graph.hpp"

#include <cassert>
#include <string>
#include <cstdio>
#include <list>
#include <random>
#include <functional>
#include <numeric>
#include <memory>
#include <omp.h>

using namespace std; // TODO: remove


void Graph::connect(unsigned type, double edgeFillDegree) {
	
	std::cout << "\nConnection Mode:\t";

	Node nd = Node(2);

	switch(type) {

		case PAPER: // Construct simple example graph from paper
			assert(N_==9);
			nodes_[0]->addChild(nodes_[2]);
			nodes_[2]->addChild(nodes_[6]);
			nodes_[6]->addChild(nodes_[3]);
			nodes_[6]->addChild(nodes_[4]);
			nodes_[3]->addChild(nodes_[5]);
			nodes_[4]->addChild(nodes_[7]);
			nodes_[7]->addChild(nodes_[5]);
			nodes_[1]->addChild(nodes_[7]);
			nodes_[8]->addChild(nodes_[1]);
			nodes_[8]->addChild(nodes_[4]);
			std::cout << "Paper";
			break;

		case RANDOM_EDGES:
		{
			assert(edgeFillDegree > 0. && edgeFillDegree <= 1.);
			// Specify (roughly) number of edges
			const int nEdges = N_ * (N_ - 1) * 0.5 * edgeFillDegree;
			int nEffectiveEdges = 0;

			// Create random order of nodes
			std::vector<unsigned> order(N_);
			std::iota(order.begin(), order.end(), 0);
			const int seed2 = 55;
			std::mt19937 gen2(seed2);
			std::shuffle(order.begin(), order.end(), gen2);

			// Prepare the random number generator
			const int seed = 42;
			std::mt19937 gen(seed);
			std::uniform_int_distribution<Graph::nodearray_type::size_type> dis(0, N_-1);
			auto rnd = std::bind(dis, gen);

			for(Graph::nodearray_type::size_type i = 0; i < nEdges; i++){
				auto rn0 = rnd();
				auto rn1 = rnd();
				
				if(rn0 == rn1)
					continue;

				// Node with lower order always points to node with higher order so as to avoid circles
				auto pointerNode = rn0;
				auto pointeeNode = rn1;
				if(order[rn0] > order[rn1]){
					pointerNode = rn1;
					pointeeNode = rn0;
				}

				if(!nodes_[pointerNode]->hasChild(nodes_[pointeeNode])){
					nodes_[pointerNode]->addChild(nodes_[pointeeNode]);
					++nEffectiveEdges;
				}
			}
			std::cout << "Created Graph with " << nEffectiveEdges << " edges." << std::endl;
			break;
		}

		default:
			std::cout << "\nERROR:\tInvalid connection index - no connections added\n";

	}

	std::cout << "\n";

}

bool Graph::checkCorrect(bool verbose) {
	
    std::cout << "\n>>Begin Check\n\n";
	bool correct = true;

    // 1. check length of solution
    if(solution_.size() != nodes_.size()){
        correct = false;
        if(verbose)
            std::cout << "ERROR: Size of solution is " << solution_.size() << ", but should be " << nodes_.size() << "\n";
    }

    // retrieve the order of each node from solution
    std::vector<size_t> nodeOrders(N_);
    std::vector<size_t> checkNodes(N_, 0);
    size_t cnt = 0;
    for(auto it = solution_.begin(); it != solution_.end(); ++it){
        size_t nodeId = (*it)->getID();
        nodeOrders[nodeId] = cnt;
        checkNodes[nodeId]++;
        ++cnt;
    }
    
    // 2. check that every node occurs exactly once in the solution
    for(size_t i = 0; i < N_; ++i){
        if(checkNodes[i] != 1){
            correct = false;
            if(verbose)
                std::cout << "ERROR: Node #" << nodes_[i]->getID() << " occurs " << checkNodes[i] << " times, but should occur exactly once.\n";
        }
    }
    
    // 3. for each (parent) node, check that each of their children has a higher sorting index
    for(size_t i = 0; i < N_; ++i){
        auto parent = nodes_[i];
        auto parentId = parent->getID();
        size_t childcount = parent->getChildCount();
        
        for(size_t k = 0; k < childcount; ++k){
            size_t childId = parent->getChild(k)->getID();
            if(nodeOrders[parentId] > nodeOrders[childId]){
                correct = false;
                if(verbose)
                    std::cout << "ERROR: Node #" << parentId << " should have lower index than node #" << childId << "\n";
            }
        }
    }

	if(correct) {
		std::cout << "OK: VALID TOPOLOGICAL SORTING.\n";
	} else {
		std::cout << "\nERROR: INVALID TOPOLOCIGAL SORTING.\n";
	}

    std::cout << "\n<<End Check\n\n";
	return correct;
}
