#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"

std::string Graph::getName(){
    return "bitset";
}

void Graph::topSort() {
	// Sorting Magic happens here
	
	type_size syncVal = 1;
	unsigned nFinished = 0;
    int nThreads;
    #pragma omp parallel
    {
        nThreads = omp_get_num_threads();
    } 
    // Indicator vector true if node is a current node (aka frontier node)
    std::vector<char> isCurrentNode(2*N_, false); //std::vector<bool> is not thread-safe
    std::vector<char> newChildrenPerThread(nThreads, true);
    bool newChildren = true;
    int shift = 0;
	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nFinished)
	{
		// Declare Thread Private Variables
		const int threadID = omp_get_thread_num();
		unsigned childcount = 0;
		unsigned currentvalue = 0;

		// Distribute Root Nodes among Threads
        #pragma omp for
		for(unsigned i=0; i<N_; ++i) {
			if(nodes_[i]->getValue()==1)
                isCurrentNode[i] = true;
		}

        A_.starttiming(analysis::BARRIER);
		#pragma omp barrier // make sure everything is set up alright
        A_.stoptiming(threadID, analysis::BARRIER);
    
        while(newChildren){
            newChildrenPerThread[threadID] = false;
            #pragma omp for schedule(dynamic, 256)
            for(size_t i = 0; i < N_; ++i){
                int idx = shift * N_ + i;
                if(!isCurrentNode[idx])
                    continue;
                
                A_.incrementProcessedNodes(threadID);

                auto parent = nodes_[i];
				currentvalue = parent->getValue();
                assert(currentvalue == syncVal);

                A_.starttiming(analysis::SOLUTIONPUSHBACK);
                #pragma omp critical
                {
                    solution_.push_back(parent); // IMPORTANT: this must be atomic
                }
                A_.stoptiming(threadID, analysis::SOLUTIONPUSHBACK);
                isCurrentNode[idx] = false;// remove current node - already visited

				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				bool flag;
				for(unsigned c=0; c<childcount; ++c) {
					auto child = parent->getChild(c);

					// Checking if last parent trying to update
                    A_.starttiming(analysis::REQUESTVALUEUPDATE);
                    flag = child->requestValueUpdate(); // This call is thread-safe
                    A_.stoptiming(threadID, analysis::REQUESTVALUEUPDATE);
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
                shift = (shift+1)%2;
                char testval = true;
                newChildren = std::find(newChildrenPerThread.begin(), newChildrenPerThread.end(), testval) != newChildrenPerThread.end();
            }
        }
	} // end of OMP parallel
    depth_ = syncVal;
}
