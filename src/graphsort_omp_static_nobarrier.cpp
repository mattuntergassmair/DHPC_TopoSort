#include <omp.h>
#include <algorithm>

#include "graph.hpp"
#include "analysis.hpp"

using type_threadcount = analysis::type_time;

std::string Graph::getName(){
    return "static_nobarrier";
}

// PRE:		globallist has all nodes that need to be distributed, locallist is empty
// POST:	locallist holds a maximum of n elements that were previously in globallist
inline void scatterlist( Graph::type_nodelist& globallist, Graph::type_nodelist& locallist, Graph::type_size n, analysis::type_threadcount id) {

	#if DEBUG >= 1
		#pragma omp single
		std::cout << "\nBEFORESCATTER -\tglobal:" << globallist;
	#endif // DEBUG >= 1

	Graph::type_nodelist::iterator start, end;
	#pragma omp critical
	{
		assert(locallist.empty());
		Graph::type_size len = globallist.size();
		n = std::min(n,len); // make sure that we do not take more nodes than present
		start = globallist.begin();
		end = globallist.begin();
		std::advance(end,n);
		locallist.splice(locallist.end(),globallist,start,end);
	}
	
	#if DEBUG >= 2
		#pragma omp single
		std::cout << "\nAFTERSCATTER -\tglobal:" << globallist;
	#endif // DEBUG >= 2
	#if DEBUG >= 1
		#pragma omp barrier
		#pragma omp critical
		std::cout << "\nAFTERSCATTER -\tthread " << id << " :" << locallist;
		#pragma omp barrier
	#endif // DEBUG >= 1

}

// PRE: 
// POST:	returns unsigned(ceil(n/d)), i.e. round-up division
inline unsigned roundupdiv(unsigned n, unsigned d) {
	return (1 + (n-1)/d);
}


void Graph::topSort() {

	// Sorting Magic happens here

	// SHARED VARIABLES
	type_nodelist currentnodes;

	// Start: currentnodes = root nodes 
	for(type_size i=0; i<N_; ++i) {
		if(nodes_[i]->getValue()==1) currentnodes.push_back(nodes_[i]);
	}
	auto nCurrentNodes = currentnodes.size();
	
	// Spawn OMP threads
	#pragma omp parallel
	{
		
		// THREAD PRIVATE VARIABLES
		const int nThreads = omp_get_num_threads();
		const int threadID = omp_get_thread_num();
		type_nodelist currentnodes_local;
		type_nodelist solution_local;
        
		A_.initialnodes(threadID,currentnodes_local.size());
		
		#pragma omp barrier // make sure everything is set up alright
        
        A_.starttiming(analysis::CURRENTSCATTER);
        scatterlist(currentnodes,currentnodes_local,roundupdiv(nCurrentNodes,nThreads), threadID);
        A_.stoptiming(threadID,analysis::CURRENTSCATTER);

        while(!currentnodes_local.empty()) {
            
            A_.incrementProcessedNodes(threadID);
            
            auto parent = currentnodes_local.front();

            A_.starttiming(analysis::SOLUTIONPUSHBACK);
            #pragma omp critical
            solution_.push_back(parent); // put node in solution
            A_.stoptiming(threadID, analysis::SOLUTIONPUSHBACK);
            currentnodes_local.pop_front(); // remove current node - already visited

            auto childcount = parent->getChildCount();
            A_.incrementProcessedEdges(threadID, childcount);
            for(type_size c=0; c<childcount; ++c) {
                auto child = parent->getChild(c);

                // Checking if last parent trying to update
                A_.starttiming(analysis::REQUESTVALUEUPDATE);
                auto flag = child->requestValueUpdate(); // IMPORTANT: control atomicity using OPTIMISTIC flag
                A_.stoptiming(threadID,analysis::REQUESTVALUEUPDATE);
                
                if(flag) { // last parent checking child
                    currentnodes_local.push_back(child); // add child node at end of queue
                } 
        
            }
        }
	} // end of OMP parallel
}
