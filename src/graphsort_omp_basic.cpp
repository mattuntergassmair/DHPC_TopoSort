#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"

std::string Graph::getName(){
    return "basic";
}

void Graph::topSort() {

	// Sorting Magic happens here
	
	unsigned syncVal = 1;
	unsigned nFinished = 0;
	std::vector<short> threadFinished;

	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nFinished)
	{
		// Create a Vector entry specifying whether thread is done or not
		#pragma omp critical
		threadFinished.push_back(false);

		// TODO: make even nicer
		// #ifdef ENABLE_ANALYSIS
		size_t n_processed_nodes = 0;
		bool hasJustFinished = true;
		// #endif

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

		A_.initialnodes(threadID,currentnodes.size());
		
		// TODO: OPTIMIZATION handle and redistribute tasks

		A_.starttiming(analysis::BARRIER);
		#pragma omp barrier // make sure everything is set up alright
        A_.stoptiming(threadID,analysis::BARRIER);
		
		unsigned i=0;
		while(i<N_ && nFinished<nThreads) {

			while(!currentnodes.empty()) {
                #ifdef ENABLE_ANALYSIS
                n_processed_nodes++; // TODO: can remove ifdef here - we are not compute bound anyways
                #endif
				parent = currentnodes.front();
				currentvalue = parent->getValue();

				if(currentvalue>syncVal) {
					assert(currentvalue == syncVal+1);
					break;
				} else {
					A_.starttiming(analysis::SOLUTIONPUSHBACK);
					#pragma omp critical 
					{
						solution_.push_back(parent); // IMPORTANT: this must be atomic
					}
					A_.stoptiming(threadID,analysis::SOLUTIONPUSHBACK);
					currentnodes.pop_front(); // remove current node - already visited
				}

				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				bool flag;
				for(unsigned c=0; c<childcount; ++c) {
					child = parent->getChild(c);

					// Checking if last parent trying to update
                    A_.starttiming(analysis::REQUESTVALUEUPDATE);
					flag = child->requestValueUpdate(); // IMPORTANT: control atomicity using OPTIMISTIC flag
                    A_.stoptiming(threadID,analysis::REQUESTVALUEUPDATE);
                    
                    if(flag) { // last parent checking child
						currentnodes.push_back(child); // add child node at end of queue
						child->setValue(currentvalue); // set value of child node to parentvalue
					} 
			
				}
			}
			threadFinished[threadID] = (currentnodes.empty() ? 1 : 0);
			#ifdef ENABLE_ANALYSIS
            if(threadFinished[threadID] == 1 && hasJustFinished){
                A_.count_LastSyncVal_[threadID] = syncVal;
                hasJustFinished = false;
            }
            #endif
            
            
			#pragma omp single
			nFinished = std::accumulate(threadFinished.begin(),threadFinished.end(),unsigned(0));

			A_.starttiming(analysis::BARRIER);
			#pragma omp barrier // make sure everything is set up alright
			A_.stoptiming(threadID,analysis::BARRIER);
			
			#pragma omp single
            ++syncVal;
			//std::cout << "\nCurrent Depth = " << ++syncVal << std::flush;
			
			++i;
		}
		A_.processednodes(threadID,n_processed_nodes);
	
	} // end of OMP parallel
}
