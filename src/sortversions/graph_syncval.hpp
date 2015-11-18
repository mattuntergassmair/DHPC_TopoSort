#ifndef GRAPH_SYNCVAL_HPP
#define GRAPH_SYNCVAL_HPP

#include "../graph.hpp"

class GraphSyncval : public DirGraph{
    using DirGraph::DirGraph;
    public:
    void topSort() {
        
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
};


#endif
