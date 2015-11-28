#include <omp.h>
#include <algorithm>

#include "graph.hpp"
#include "analysis.hpp"

using type_threadcount = analysis::type_time;

template<typename T>
inline void gatherlist(std::list<T> globallist, std::list<T> locallist, analysis::type_threadcount id) {
	#pragma omp critical
	{
		globallist.splice(globallist.end(),locallist);
	}
}

template<typename T>
inline void scatterlist(std::list<T> globallist, std::list<T> locallist, Graph::type_size n, analysis::type_threadcount id) {

	#if VERBOSE >= 2
		#pragma omp critical
		{
			std::cout << "SCAT - TID:" << id;
		}
	#endif // VERBOSE >= 2

	#pragma omp critical
	{
		Graph::type_size len = globallist.size();
		n = std::min(n,len); // make sure that we do not take more nodes than present
		typename std::list<T>::const_iterator start, end;
		start = globallist.begin();
		end = globallist.begin();
		std::advance(end,n);;
		assert(locallist.empty());
		// include assertion making sure that all values in currentnodes are the same; // TODO:
		// NB: this is checked by correctness check anyways
		locallist.splice(locallist.end(),globallist,start,end);
	}
}

// PRE: 
// POST: returns unsigned(ceil(n/d)), i.e. round-up division
inline unsigned roundupdiv(unsigned n, unsigned d) {
	return (1 + (n-1)/d);
}




void Graph::topSort() {

	// Sorting Magic happens here

	// SHARED VARIABLES
	type_size syncVal = 1;
	type_size nCurrentNodes = 0;
	type_nodelist currentnodes;

	// Start: currentnodes = root nodes 
	for(type_size i=0; i<N_; ++i) {
		if(nodes_[i]->getValue()==1) currentnodes.push_back(nodes_[i]);
	}
	nCurrentNodes = currentnodes.size();

	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nCurrentNodes, currentnodes)
	{
		
		// THREAD PRIVATE VARIABLES
		const int nThreads = omp_get_num_threads();
		const int threadID = omp_get_thread_num();
		type_nodelist currentnodes_local;
		type_nodelist solution_local;
		
		type_nodeptr parent;
		type_nodeptr child;
		type_size childcount = 0;
		type_size currentvalue = 0;

		A_.starttiming(analysis::CURRENTSCATTER);
		scatterlist<type_nodeptr>(currentnodes,currentnodes_local,roundupdiv(nCurrentNodes,nThreads), threadID);
		A_.stoptiming(threadID,analysis::CURRENTSCATTER);

		A_.initialnodes(threadID,currentnodes_local.size());
		
		A_.starttiming(analysis::BARRIER);
		#pragma omp barrier // make sure everything is set up alright
        A_.stoptiming(threadID,analysis::BARRIER);
		
		type_size i=0;

		while(i<N_ && !currentnodes.empty()) {

			#pragma omp single
			{
				nCurrentNodes = currentnodes.size();
			}
			A_.starttiming(analysis::BARRIER);
			#pragma omp barrier // make sure that nCurrentNodes is set
			A_.stoptiming(threadID,analysis::BARRIER);
		
			A_.starttiming(analysis::CURRENTSCATTER);
			scatterlist<type_nodeptr>(currentnodes,currentnodes_local,roundupdiv(nCurrentNodes,nThreads), threadID);
			A_.stoptiming(threadID,analysis::CURRENTSCATTER);

			while(!currentnodes_local.empty()) {
                
                A_.incrementProcessedNodes(threadID);
				
				parent = currentnodes_local.front();
				currentvalue = parent->getValue();

				if(currentvalue>syncVal) {
					assert(currentvalue == syncVal+1);
					break;
				} else {
					solution_local.push_back(parent); // put node in solution
					currentnodes_local.pop_front(); // remove current node - already visited
				}

				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				bool flag;
				for(type_size c=0; c<childcount; ++c) {
					child = parent->getChild(c);

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

			// Collect local lists in global list
			// TODO: implement timing

			A_.starttiming(analysis::CURRENTGATHER);
			gatherlist<type_nodeptr>(currentnodes,currentnodes_local,threadID);
			A_.stoptiming(threadID,analysis::CURRENTGATHER);
			
			// TODO: nowait
			A_.starttiming(analysis::SOLUTIONPUSHBACK);
			gatherlist<type_nodeptr>(solution_,solution_local,threadID);
			A_.stoptiming(threadID,analysis::SOLUTIONPUSHBACK);

			A_.starttiming(analysis::BARRIER);
			#pragma omp barrier // make sure everything is set up alright
			A_.stoptiming(threadID,analysis::BARRIER);
			
			#pragma omp single
            ++syncVal;
            // TODO: implement with verbosity
			//std::cout << "\nCurrent Depth = " << ++syncVal << std::flush;
			
			++i; // TODO: finally remove i counter when stable
		}
	
	} // end of OMP parallel

}
