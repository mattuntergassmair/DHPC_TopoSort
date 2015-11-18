#ifndef GRAPHSORT_OMPTASKS_HPP
#define GRAPHSORT_OMPTASKS_HPP

#include "graph.hpp"

class GraphSort_OMP_Tasks : public Graph {
    using Graph::Graph;
    public:
    
    void topSort(){
	// Spawn OMP threads
	#pragma omp parallel
	{

		#pragma omp single
		{
			// TODO: PRIORITY consider starting parallel section here so each thread has own list
			// Sorting Magic happens here
			std::list<std::shared_ptr<Node> > currentnodes;
			for(unsigned i=0; i<N_; ++i) { // TODO: OPTIONAL smart way for initial distribution
				// FOR EXAMPLE: node id % num threads
				if(nodes_[i]->getValue() == 1) currentnodes.push_back(nodes_[i]);
			}
			std::shared_ptr<Node> parent;
			std::shared_ptr<Node> child;
			unsigned childcount = 0;
			unsigned currentvalue = 0;

			// TODO: OPTIMIZATION handle and redistribute tasks
		
			#pragma omp task
			while(!currentnodes.empty()) { // stop when queue is empty

				parent = currentnodes.front();
				currentnodes.pop_front(); // remove current node - already visited
				currentvalue = parent->getValue();
				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				#pragma omp critical
				{ // IMPORTANT: THIS MUST BE ATOMIC
					solution_.push_back(parent);
				}

				for(unsigned c=0; c<childcount; ++c) {
					child = parent->getChild(c);
					// TODO: PRIORITY requestValueUpdate must be atomic
					if(child->requestValueUpdate()) { // last parent checking child
						currentnodes.push_back(child); // add child node at end of queue
						child->setValue(currentvalue); // set value of child node to parentvalue
					} else {
						// do nothing
					}
				}

				// TODO: synchronization must happen here (or somewhere) ;)
				// omp barrier				
				#pragma omp taskwait

			}

		} // end of single OMP section
	} // end of parallel section        
        
    }
};


#endif //  GRAPHSORT_OMPTASKS_HPP
