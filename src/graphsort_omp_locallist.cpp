#include <omp.h>

#include "graph.hpp"
#include "analysis.hpp"

using type_threadcount = analysis::type_time;

// // TODO: template might be an overkill here
// template<typename T>
// void gatherlist(std::list<T> locallist, std::list<T> globallist, analysis::type_threadcount id) {
// 	#pragma omp critical
// 	{
// 		globallist.splice(globallist.end(),locallist);
// 	}
// }

// template<typename T>
// void scatterlist(std::list<T> locallist, std::list<T> globallist, analysis::type_threadcount n) {
// 	#pragma omp critical
// 	{
// 		analysis::type_size len = globallist.size();
// 		n = std::min(n,len);
// 		assert(locallist.empty());
// 		// include assertion making sure that all values in currentnodes are the same; // TODO:
// 		locallist.splice(locallist.end(),globallist,globallist.begin(),globallist.begin()+n);
// 	}
// }


// TODO: improve analysis functionality - exclude all ifdefs form graphsort


void Graph::topSort() {

	// // Sorting Magic happens here

	// // SHARED VARIABLES
	// type_size syncVal = 1;
	// type_threadcount nFinished = 0;
	// type_size nCurrentNodes = 0;
	// std::vector<type_threadcount> threadFinished;
	// type_nodelist currentnodes;

	// // Start: currentnodes = root nodes 
	// for(type_size i=0; i<N_; ++i) {
	// 	if(nodes_[i]->getValue()==1) currentnodes.push_back(nodes_[i]);
	// }
	// nCurrentNodes = currentnodes.size();

	// // TODO: check - can you make currentnodes shared? do you even need to?
	// // make threadFinished shared (this could have been problem of implementation)
	// // this might be the root of all evil

	// // Spawn OMP threads
	// #pragma omp parallel shared(syncVal, nFinished, nCurrentNodes)
	// {
	// 	// Create a Vector entry specifying whether thread is done or not
	// 	#pragma omp critical
	// 	threadFinished.push_back(false);

	// 	// THREAD PRIVATE VARIABLES
	// 	const int nThreads = omp_get_num_threads();
	// 	const int threadID = omp_get_thread_num();
	// 	type_nodelist currentnodes_local;
	// 	type_nodelist solution_local;
		
	// 	type_nodeptr parent;
	// 	type_nodeptr child;
	// 	type_size childcount = 0;
	// 	type_size currentvalue = 0;

	// 	A_.initialnodes(threadID,currentnodes.size());
		
	// 	// TODO: OPTIMIZATION handle and redistribute tasks

	// 	A_.starttiming(analysis::BARRIER);
	// 	#pragma omp barrier // make sure everything is set up alright
        // A_.stoptiming(threadID,analysis::BARRIER);
		
	// 	type_size i=0;
	// 	while(i<N_ && nFinished<nThreads) {

	// 		while(!currentnodes.empty()) {
                
                // A_.incrementProcessedNodes(threadID);
				
	// 			parent = currentnodes.front();
	// 			currentvalue = parent->getValue();

	// 			if(currentvalue>syncVal) {
	// 				assert(currentvalue == syncVal+1);
	// 				break;
	// 			} else {
	// 				A_.starttiming(analysis::SOLUTIONPUSHBACK);
	// 				#pragma omp critical 
	// 				{
	// 					solution_.push_back(parent); // IMPORTANT: this must be atomic
	// 				}
	// 				A_.stoptiming(threadID,analysis::SOLUTIONPUSHBACK);
	// 				currentnodes.pop_front(); // remove current node - already visited
	// 			}

	// 			++currentvalue; // increase value for child nodes
	// 			childcount = parent->getChildCount();

	// 			bool flag;
	// 			for(type_size c=0; c<childcount; ++c) {
	// 				child = parent->getChild(c);

	// 				// Checking if last parent trying to update
                    // A_.starttiming(analysis::REQUESTVALUEUPDATE);
	// 				flag = child->requestValueUpdate(); // IMPORTANT: control atomicity using OPTIMISTIC flag
                    // A_.stoptiming(threadID,analysis::REQUESTVALUEUPDATE);
                    
                    // if(flag) { // last parent checking child
	// 					currentnodes.push_back(child); // add child node at end of queue
	// 					child->setValue(currentvalue); // set value of child node to parentvalue
	// 				} 
			
	// 			}
	// 		}
	// 		threadFinished[threadID] = (currentnodes.empty() ? 1 : 0);
            
            
	// 		#pragma omp single
	// 		nFinished = std::accumulate(threadFinished.begin(),threadFinished.end(),type_threadcount(0));

	// 		A_.starttiming(analysis::BARRIER);
	// 		#pragma omp barrier // make sure everything is set up alright
	// 		A_.stoptiming(threadID,analysis::BARRIER);
			
	// 		#pragma omp single
            // ++syncVal;
	// 		//std::cout << "\nCurrent Depth = " << ++syncVal << std::flush;
			
	// 		++i;
	// 	}
	
	// } // end of OMP parallel
}
