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

using namespace std;

void DirGraph::topSort() {
	
	// Sorting Magic happens here
	
	unsigned syncVal = 1;
	unsigned nFinished;
	std::vector<short> threadFinished;

	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nFinished)
	{

		// Create a Vector entry specifying whether thread is done or not
		#pragma omp critical
		threadFinished.push_back(false);

		// Declare Thread Private Variables
		const int nThreads = omp_get_num_threads();
		const int threadID = omp_get_thread_num();
		std::list<std::shared_ptr<Node> > currentnodes;
		
		std::shared_ptr<Node> parent;
		std::shared_ptr<Node> child;
		unsigned childcount = 0;
		unsigned currentvalue = 0;

		// Distribute Root Nodes among Threads
		for(unsigned i=0; i<N_; ++i) {
			// TODO: find smarter way for distributing nodes
			if(nodes_[i]->getValue()==1 && i%nThreads==threadID) currentnodes.push_back(nodes_[i]);
		}

		// TODO: OPTIMIZATION handle and redistribute tasks
	
		#pragma omp barrier // make sure everything is set up alright

		unsigned i=0;
		while(i<N_ && nFinished<nThreads) {

			while(!currentnodes.empty()) {

				parent = currentnodes.front();
				currentvalue = parent->getValue();

				if(currentvalue>syncVal) {
					assert(currentvalue == syncVal+1);
					break;
				} else {
					#pragma omp critical 
					{
						solution_.push_back(parent); // IMPORTANT: this must be atomic
					}
					currentnodes.pop_front(); // remove current node - already visited
				}

				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				bool flag;
				for(unsigned c=0; c<childcount; ++c) {
					child = parent->getChild(c);

					// Checking if last parent trying to update
					#pragma omp critical
					{
						flag = child->requestValueUpdate(); // IMPORTANT: this must be atomic
					}
					if(flag) { // last parent checking child
						currentnodes.push_back(child); // add child node at end of queue
						child->setValue(currentvalue); // set value of child node to parentvalue
					} 
			
				}
			}
			threadFinished[threadID] = (currentnodes.empty() ? 1 : 0);
			
			#pragma omp single
			nFinished = std::accumulate(threadFinished.begin(),threadFinished.end(),unsigned(0));

			#pragma omp barrier
			#pragma omp single
			std::cout << "\nCurrent Depth = " << ++syncVal << std::flush;
			
			++i;
		}
	
	} // end of OMP parallel
}


void DirGraph::connect(unsigned type, double edgeFillDegree) {
	
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
			std::uniform_int_distribution<DirGraph::nodearray_type::size_type> dis(0, N_-1);
			auto rnd = std::bind(dis, gen);

			for(DirGraph::nodearray_type::size_type i = 0; i < nEdges; i++){
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

bool DirGraph::checkCorrect() {
	
	bool correct = true;

    // retrieve the order of each node from solution
    std::vector<size_t> nodeOrders(N_);
    size_t cnt = 0;
    for(auto it = solution_.begin(); it != solution_.end(); ++it){
        size_t nodeId = (*it)->getID();
        nodeOrders[nodeId] = cnt;
        ++cnt;
    }
    
    // for each (parent) node, check that each of their children has a higher sorting index
    for(size_t i = 0; i < N_; ++i){
        auto parent = nodes_[i];
        auto parentId = parent->getID();
        size_t childcount = parent->getChildCount();
        
        for(size_t k = 0; k < childcount; ++k){
            size_t childId = parent->getChild(k)->getID();
            if(nodeOrders[parentId] > nodeOrders[childId]){
                correct = false;
            }
        }
    }

	if(correct) {
		std::cout << "\n\nOK: VALID TOPOLOGICAL SORTING.\n";
	} else {
		std::cout << "\n\nERROR: INVALID TOPOLOCIGAL SORTING.\n\n";
	}

	return correct;
}
