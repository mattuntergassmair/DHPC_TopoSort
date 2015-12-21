#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"

std::string Graph::getName(){
    return "dynamic_nobarrier";
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
    std::vector<char> isCurrentNode(N_, false); //std::vector<bool> is not thread-safe
	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nFinished)
	{
		// Declare Thread Private Variables
		const int threadID = omp_get_thread_num();
		unsigned childcount = 0;
		unsigned currentvalue = 0;
		type_nodelist currentnodes_local;
		type_nodelist solution_local;

		// Distribute Root Nodes among Threads
        #pragma omp for
		for(unsigned i=0; i<N_; ++i) {
			if(nodes_[i]->getValue()==1)
                isCurrentNode[i] = true;
		}
    
            
        #pragma omp for schedule(dynamic, 1024)
        for(size_t i = 0; i < N_; ++i){
            if(!isCurrentNode[i])
                continue;
            
            // Each thread on its own
            currentnodes_local.push_back(nodes_[i]);
            while(!currentnodes_local.empty()) {
                
                A_.incrementProcessedNodes(threadID);
                
                auto parent = currentnodes_local.front();
                currentvalue = parent->getValue();

                A_.starttiming(analysis::SOLUTIONPUSHBACK);
                #pragma omp critical
                solution_.push_back(parent); // put node in solution
                A_.stoptiming(threadID, analysis::SOLUTIONPUSHBACK);
                currentnodes_local.pop_front(); // remove current node - already visited


                ++currentvalue; // increase value for child nodes
                childcount = parent->getChildCount();

                bool flag;
                for(type_size c=0; c<childcount; ++c) {
                    auto child = parent->getChild(c);

                    // Checking if last parent trying to update
                    A_.starttiming(analysis::REQUESTVALUEUPDATE);
                    flag = child->requestValueUpdate(); // IMPORTANT: control atomicity using OPTIMISTIC flag
                    A_.stoptiming(threadID,analysis::REQUESTVALUEUPDATE);
                    
                    if(flag) { // last parent checking child
                        currentnodes_local.push_back(child); // add child node at end of queue
                        child->setValue(currentvalue); // set value of child node to parentvalue
                    } 
            
                }
            }
        }
	} // end of OMP parallel
}
