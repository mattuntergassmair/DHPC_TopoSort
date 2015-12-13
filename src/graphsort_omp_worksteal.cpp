#include <omp.h>
#include <algorithm>

#include "graph.hpp"
#include "analysis.hpp"

using type_threadcount = analysis::type_threadcount;

std::string Graph::getName(){
    return "worksteal";
}

// PRE:		
// POST:	locallist is appended to globallist, locallist is empty
inline void gatherlist(Graph::type_nodelist& globallist, Graph::type_nodelist& locallist, analysis::type_threadcount id) {
	#if DEBUG >= 2
		#pragma omp single
		std::cout << "\nBEFOREGATHER -\tglobal:" << globallist;
		#pragma omp barrier
		#pragma omp critical
		std::cout << "\nBEFOREGATHER -\tthread " << id << " :" << locallist;
		#pragma omp barrier
	#endif // DEBUG >= 2
	
	#pragma omp critical
	{
		globallist.splice(globallist.end(),locallist);
	}
	assert(locallist.empty());

	#if DEBUG >= 2
		#pragma omp single
		std::cout << "\nAFTERGATHER -\tglobal:" << globallist;
	#endif // DEBUG >= 2
}

/*
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
*/

// PRE: 
// POST:	returns unsigned(ceil(n/d)), i.e. round-up division
inline unsigned roundupdiv(unsigned n, unsigned d) {
	return (1 + (n-1)/d);
}


namespace myworksteal {

	class nodePool; // Forward declaration of class

	class threadLocallist {

		public:

			threadLocallist(nodePool& np)
				: np_(np)
				, tid_(-1)
				, locallist_current_fast_()
				, locallist_current_stack_()
				, locallist_next_()
				, solution_local_()
				, currentSyncVal_(0)
			{}

			// TRY TO STEAL A NODE FROM BUSY THREAD
			// PRE: 
			// POST: returns a pointer of a node to work on or nullptr if request was unsuccessful
			inline std::shared_ptr<Node> trySteal() {
				std::shared_ptr<Node> nd = nullptr;
				#pragma omp critical
				{
					std::cout << "\nStealing one node from thread " << tid_;
					if(!locallist_current_stack_.empty()) {
						nd = locallist_current_stack_.back();
						locallist_current_stack_.pop_back();
					}
				}
				return nd;
			}

			void work(type_threadcount tid); // implementation below


		private:

			nodePool& np_;
			analysis::type_threadcount tid_;
			Graph::type_nodelist locallist_current_fast_;
			Graph::type_nodelist locallist_current_stack_;
			Graph::type_nodelist locallist_next_;
			Graph::type_nodelist solution_local_;
			Graph::type_size currentSyncVal_;

			friend nodePool;

			// Advances to the next sync value
			// PRE:		locallist_current_* must be empty, the nodes contained in locallist_next_ must have the correct value nsv
			// POST:	swaps the locallist_current_stack_ contains new nodes to work on, locallist_next_ is empty, currenySyncVal_ is set to nsv
			inline void nextSyncVal(Graph::type_size nsv) {
				assert(locallist_current_fast_.empty() && locallist_current_stack_.empty()); // make sure old lists are empty
				assert(locallist_next_.empty() || locallist_next_.front()->getValue()==nsv); // make sure value is set to correct sync value
				currentSyncVal_ = nsv;
				std::swap(locallist_current_stack_,locallist_next_);
				assert(locallist_next_.empty());
				// stackToFast(); // TODO: this should be called somewhere else
			}
			
			// moves nodes from the stack to the fast list critically
			inline void stackToFast() {
				assert(locallist_current_fast_.empty());
				// TODO: critically transfer a "reasonable" portion from stack to fast
				// What is "reasonable"?
				// Experiment with different parameters:
				// - count of threads currently done
				// - ...
				#pragma omp critical 
				{
					// TODO: make better
					// Naive:
					Graph::type_nodelist::const_iterator start = locallist_current_stack_.begin();
					Graph::type_nodelist::const_iterator end = locallist_current_stack_.begin();
					if(end != locallist_current_stack_.end()) {
						// TODO: make this faster
						std::advance(end,1); // take one element only - this will be slow, but hopefully correct
						locallist_current_fast_.splice(locallist_current_fast_.end(),locallist_current_stack_,start,end);
					}
				}
			}
			
			inline bool noMoreNodes() {
				// TODO: assertions might be to strong here
				// assert(locallist_current_fast_.empty());
				// assert(locallist_current_stack_.empty());
				// return !locallist_next_.empty();
				// TODO: this might not be threadsafe, imagine someone is stealing ...
				return (locallist_current_fast_.empty() && locallist_current_stack_.empty() && locallist_next_.empty());
			}


			// used when distributing initial nodes
			// PRE:		ndptr points to valid node
			// POST:	ndptr is inserted into locallist_next_
			inline void insert(Graph::type_nodeptr ndptr) {
				locallist_next_.push_back(ndptr);
			}

			friend std::ostream& operator<<(std::ostream&, threadLocallist&);

	};

	#if DEBUG>0 || VERBOSE>0
	// This is only needed for debugging
	std::ostream& operator<<(std::ostream& os, threadLocallist& tll) {
		os << "\n=================================\nTID: " << tll.tid_;
		os << "\n\tFAST: " << tll.locallist_current_fast_;
		os << "\n\tSTACK: " << tll.locallist_current_stack_;
		os << "\n\tNEXT: " << tll.locallist_next_;
		os << "\n\tSOLUTION: " << tll.solution_local_;
		os << "\n=================================\n";
		return os;
	}
	#endif // DEBUG>0 || VERBOSE>0
	
	
	class nodePool {

		public:
		
			// PRE: must call constructor single threaded
			nodePool(Graph::type_size nThreads, Graph::type_nodelist& sollist)
				: nThreads_(nThreads)
				, globalsolution_(sollist)
				, nDoneWithSyncVal_(0)
				, doneWithSyncVal_(nThreads_,0)
				, nodelists_(nThreads_,myworksteal::threadLocallist(*this))
			{
				std::cout << "\n\nInitialized thread-locallists for " << nThreads_ << " threads:\n\n";
				for(type_threadcount i=0; i<nThreads_; ++i) {
		#if DEBUG>0 || VERBOSE>0
					#pragma omp critical
					{
						std::cout << nodelists_[i];
					}
		#endif // DEBUG>0 || VERBOSE>0
				}
			}

			// PRE:
			// POST:	doneWithSyncVal_[i] = 0 for all i
			void setUndoneAll() {
				for(auto d : doneWithSyncVal_) d = 0;
			}

			// TODO: check if this works well
			void setDoneWithSyncVal(type_threadcount i) {
				#pragma omp critical // TODO: not sure if this is necessary
				{
					doneWithSyncVal_[i] = 1;
					nDoneWithSyncVal_ = std::accumulate(doneWithSyncVal_.begin(),doneWithSyncVal_.end(),type_threadcount(0));
				}
			}

			type_threadcount getNThreads() const {
				return nThreads_;
			}

			type_threadcount countDone() /*const*/ { // TODO: const can be reintroduced later	
				// TODO: this can be removed cause impelemented above - just safety for now
				nDoneWithSyncVal_ = std::accumulate(doneWithSyncVal_.begin(),doneWithSyncVal_.end(),type_threadcount(0)); 
				return nDoneWithSyncVal_;
			}

			bool allDoneWithSyncVal() /*const*/ { // TODO: const can be reintroduced later	
				#pragma omp single
				std::cout << "\n\n" << nDoneWithSyncVal_ << " threads are done with current step\n";
				return countDone()>=nThreads_;
			}
		
			// PRE:		nsv is the valid next SyncValue (gradually incrementing)
			// POST:	all local threadlists are updated to the new SyncValue nsv
			void nextSyncVal(Graph::type_size nsv) {
				for(auto nd : nodelists_) {
					nd.nextSyncVal(nsv);
				}
			}

			// PRE:		ndptr is a valid node pointer, i is a valid thread index
			// POST:	ndptr is inserted in the local list of thread tid
			inline void insertNode(Graph::type_nodeptr ndptr, type_threadcount tid) {
				assert(tid<nThreads_);
				nodelists_[tid].insert(ndptr);
			}

			bool sortingComplete() {
				for(auto nd : nodelists_) {
					if(!nd.noMoreNodes()) return false;
				}
				return true;
			}

			// spawns threads and synchronizes them
			Graph::type_nodelist& workparallel(Graph& graph) {
				
				// SHARED VARIABLES
				Graph::type_size syncVal = 0;

				// Spawn OMP threads
				#pragma omp parallel shared(syncVal)
				{

					#pragma omp single
					std::cout << "Working in parallel with " << omp_get_num_threads() << " threads (nthreads=" << nThreads_ << ")" << std::flush;
					assert(nThreads_==omp_get_num_threads()); // TODO: this may need to be removed

					// THREAD PRIVATE VARIABLES
					const int threadID = omp_get_thread_num();
		
					#pragma omp barrier

					do {
						
						#pragma omp single
						setUndoneAll();
					
						#pragma omp single
						++syncVal;
						
						nodelists_[threadID].nextSyncVal(syncVal);
						
						#if VERBOSE>=2
						#pragma omp single
						std::cout << "\nCurrent syncVal = " << syncVal;
						#endif // VERBOSE>=2
		
						nodelists_[threadID].work(threadID);
						#pragma omp barrier

						#pragma omp single
						std::cout << *this;

					} while(syncVal<20 && !sortingComplete()); // TODO: find appropriate terminaiton condition
				
				} // end of OMP parallel
				graph.setDepth(syncVal);

			}

			inline Graph::type_nodeptr tryStealFrom(type_threadcount tid) {
				return nodelists_[tid].trySteal();
			}


		private:
			const type_threadcount nThreads_;
			Graph::type_nodelist& globalsolution_;
			type_threadcount nDoneWithSyncVal_;
			std::vector<type_threadcount> doneWithSyncVal_;
			std::vector<myworksteal::threadLocallist> nodelists_;

		friend std::ostream& operator<<(std::ostream&, nodePool&);
		friend class threadLocallist;
	};

	#if DEBUG>0 || VERBOSE>0
	std::ostream& operator<<(std::ostream& os, nodePool& ndp) {
		for(type_threadcount i=0; i<ndp.nThreads_; ++i) {
			std::cout << "\n\nCurrent state of ThreadPool:\n";
			std::cout << ndp.nodelists_[i];
		}
	}
	#endif // DEBUG>0 || VERBOSE>0

	
	
	// functions with dependencies on pool go here

	void threadLocallist::work(type_threadcount tid) {

		// make sure this is always called with the same thread-id
		assert(tid_==tid || tid_==-1);
		tid_ = tid;

		assert(locallist_next_.empty());

		Graph::type_nodeptr parent;
		Graph::type_nodeptr child;
		Graph::type_size childcount = 0;

		#pragma omp barrier // make sure everything is set up alright
	
		// A_.incrementProcessedNodes(threadID); // TODO: this is a bonus

		// TODO: remove this
		#pragma omp single
		std::cout << "\n\nBEFORE CLEAR "<< *this;
		
		locallist_current_stack_.clear();
		locallist_current_fast_.clear();
		locallist_next_.clear();
		
		#pragma omp single
		std::cout << "\n\nAFTER CLEAR "<< *this;
		
		
		do {
			while(!locallist_current_stack_.empty()) {
				while(!locallist_current_fast_.empty()) {
				
					parent = locallist_current_fast_.front();
					assert(parent->getValue() == currentSyncVal_); // TODO: this can be removed once working
				
					solution_local_.push_back(parent); // put node in solution
					locallist_current_fast_.pop_front(); // remove current node - already visited
					
					// TODO: implement work here
					//
					//
					//
					//
					//
					//
				}
				stackToFast();
			}
			// TODO: trysteal goes here, but don't steal for now
			np_.setDoneWithSyncVal(tid_);
			// if(trySteal()!=nullptr);
		} while(!np_.allDoneWithSyncVal());

		// Collect local lists in global list
		gatherlist(np_.globalsolution_,solution_local_,tid_);

		/*
		
		

		} else {
		}

		++currentvalue; // increase value for child nodes
		childcount = parent->getChildCount();

		bool flag;
		for(type_size c=0; c<childcount; ++c) {
			child = parent->getChild(c);

			// Checking if last parent trying to update
			flag = child->requestValueUpdate(); // IMPORTANT: control atomicity using OPTIMISTIC flag
			
			if(flag) { // last parent checking child
				currentnodes_local.push_back(child); // add child node at end of queue
				child->setValue(currentvalue); // set value of child node to parentvalue
			} 
	
		}

		
			
	*/

	}


}





	// TODO: steal randomly from others
	// TODO: find out how to sync


void Graph::topSort() {

	std::cout << "\n\n\nRUNNING IN MODE: d-" << DEBUG << " - v-" << VERBOSE << "\n\n\n";
	#if DEBUG>0 || VERBOSE>0
	std::cout << "\n\n\nRUNNING IN DEBUG MODE\n\n\n";
	#endif // DEBUG>0 || VERBOSE>0

	myworksteal::nodePool nodepool(this->A_.nThreads_,this->solution_);

	// Sorting Magic happens here


	// Start: currentnodes = root nodes 
	type_threadcount i=0;
	for(auto nd : nodes_) {
		if(nd->getValue()==1) {
			nodepool.insertNode(nd,i);
			++i;
			i %= nodepool.getNThreads();
		}
	}

	#if DEBUG>0 || VERBOSE>0
	std::cout << nodepool;
	#endif // DEBUG>0 || VERBOSE>0


	nodepool.workparallel(*this);


}
