#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"

std::string Graph::getName(){
    return "bitset";
}

void Graph::topSort() {

    #ifdef ENABLE_ANALYSIS
    util::rdtsc_timer rt_total;
    rt_total.start();
    #endif
	// Sorting Magic happens here
	
	unsigned syncVal = 1;
	unsigned nFinished = 0;
    int nThreads;
    #pragma omp parallel
    {
        nThreads = omp_get_num_threads();
    }
    // Create a Vector entry specifying whether thread is done or not
	std::vector<bool> threadFinished(nThreads, false);
    
    // Indicator vector true if node is a current node (aka frontier node)
    std::vector<bool> isCurrentNode(2*N_, false);
    std::vector<bool> newChildrenPerThread(nThreads, true);
    bool newChildren = true;
    int shift = 0;
	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nFinished)
	{
        #ifdef ENABLE_ANALYSIS
        util::rdtsc_timer rt_barrier;
        util::rdtsc_timer rt_solution_pushback;
        util::rdtsc_timer rt_requestValueUpdate;
        size_t n_processed_nodes = 0;
        bool hasJustFinished = true;
        #endif

		// Declare Thread Private Variables
		const int threadID = omp_get_thread_num();
		unsigned childcount = 0;
		unsigned currentvalue = 0;

		// Distribute Root Nodes among Threads
        #pragma omp for
		for(unsigned i=0; i<N_; ++i) {
			// TODO: find smarter way for distributing nodes
			if(nodes_[i]->getValue()==1)
                isCurrentNode[i] = true;
		}

		// TODO: OPTIMIZATION handle and redistribute tasks
        #ifdef ENABLE_ANALYSIS
        rt_barrier.start();
        #endif
		#pragma omp barrier // make sure everything is set up alright
        #ifdef ENABLE_ANALYSIS
        rt_barrier.stop();
        analysis_.t_barrier[threadID] += rt_barrier.sec();
        #endif
        
        while(newChildren){
            newChildrenPerThread[threadID] = false;
            #pragma omp for schedule(dynamic, 256)
            for(size_t i = 0; i < N_; ++i){
                int idx = shift * N_ + i;
                if(!isCurrentNode[idx])
                    continue;
            
                #ifdef ENABLE_ANALYSIS
                n_processed_nodes++;
                #endif
				auto parent = nodes_[i];
				currentvalue = parent->getValue();

				if(currentvalue>syncVal) {
					assert(currentvalue == syncVal+1);
                    newChildrenPerThread[threadID] = true;
                    isCurrentNode[idx] = false;
                    isCurrentNode[((shift+1)%2) * N_ + i] = true;
                    continue;
				} else {
                    #ifdef ENABLE_ANALYSIS
                    rt_solution_pushback.start();
                    #endif
                    #pragma omp critical
                    {
                        solution_.push_back(parent); // IMPORTANT: this must be atomic
                    }
                    #ifdef ENABLE_ANALYSIS
                    rt_solution_pushback.stop();
                    analysis_.t_solution_pushback[threadID] += rt_solution_pushback.sec();
                    #endif
                    isCurrentNode[idx] = false;// remove current node - already visited
				}

				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				bool flag;
				for(unsigned c=0; c<childcount; ++c) {
					auto child = parent->getChild(c);

					// Checking if last parent trying to update
                    #ifdef ENABLE_ANALYSIS
                    rt_requestValueUpdate.start();
                    #endif
                        
                    #pragma omp critical
                    {
						flag = child->requestValueUpdate(); // IMPORTANT: this must be atomic
					}
                    #ifdef ENABLE_ANALYSIS
                    rt_requestValueUpdate.stop();
                    analysis_.t_requestValueUpdate[threadID] += rt_requestValueUpdate.sec();
                    #endif
                    if(flag) { // last parent checking child
                        newChildrenPerThread[threadID] = true;
                        isCurrentNode[((shift+1)%2) * N_ + child->getID()] = true;// mark child as queued
						child->setValue(currentvalue); // set value of child node to parentvalue
					} 
				}
			}// end for => one frontier completed
			#pragma omp single
            {
                ++syncVal;
                //std::cout << "\nCurrent Depth = " << syncVal << std::flush;
                shift = (shift+1)%2;
                newChildren = std::find(newChildrenPerThread.begin(), newChildrenPerThread.end(), true) != newChildrenPerThread.end();
            }
        }
        #ifdef ENABLE_ANALYSIS
            analysis_.n_processed_nodes[threadID] = n_processed_nodes;
        #endif
	} // end of OMP parallel
    
#ifdef ENABLE_ANALYSIS
    rt_total.stop();
    analysis_.t_total = rt_total.sec();
#endif
}
