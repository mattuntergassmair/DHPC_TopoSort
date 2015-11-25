// #ifndef ANALYSIS_HPP
// #define ANALYSIS_HPP

#ifdef ENABLE_ANALYSIS
#include "rdtsc_timer.hpp"
#endif

#include <vector>
#include <map>
#include <ostream>


// TODO: check nested include guards


#ifdef ENABLE_ANALYSIS
struct analysis {

	// TYPES AND VARIABLES
	
	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,N_TIMECAT};

	using type_time = double;
	using type_size = unsigned;
	using type_countmap = std::map<unsigned,type_size>;
	using type_timingmap = std::map<unsigned,type_time>;
	using type_timingvector = std::vector<type_timingmap>;
	using type_clockvector = std::vector<util::rdtsc_timer>;

	analysis()
		:	count_InitialNodes_()
		,	count_ProcessedNodes_()
		,	count_LastSyncVal_()
		,	time_Total_(0)
		,	nThreads_(0)
		,	clocks_(N_TIMECAT)
		,	timings_(N_TIMECAT,type_timingmap())
	{}

	type_countmap count_InitialNodes_;		// counts how many nodes each thread has initially
	type_countmap count_ProcessedNodes_;	// counts how many nodes each thread has processed in total
	type_countmap count_LastSyncVal_;		// keeps track of the last sync value of each thread
	type_timingvector timings_;
	type_time time_Total_;
	unsigned nThreads_;
	type_clockvector clocks_; // (10); // N_TIMECAT); // TODO: check


	// FUNCTIONS
	
	inline void initialnodes(unsigned tid, unsigned nNodes) {
		count_InitialNodes_[tid] = nNodes;
	};
	
	inline void processednodes(unsigned tid, unsigned nNodes) {
		count_ProcessedNodes_[tid] = nNodes;
	};

	inline void starttiming(timecat c) {
		clocks_[c].start();
	}

	inline void stoptiming(unsigned tid, timecat c) {
		// c stands for the index of the time category we are measuring
		// tid is the thread id
		clocks_[c].stop(); // stop timing
		timings_[c][tid] += clocks_[c].sec(); // get time in seconds and add to total (for given thread)
	}

	inline void threadcount(unsigned n) {
		nThreads_ = n;
	}


    void printAnalysis(std::ostream& out){
       
       	// TODO: discuss - maybe better to sum over all elements, not take maximum
       	// NOTE: max_element returns pointer to maximum element over all threads
       	// second extracts the timing value from the key-value pair
        type_time tmax_barrier = (std::max_element(timings_[BARRIER].begin(), timings_[BARRIER].end()))->second;
        type_time tmax_solutionPushback = (std::max_element(timings_[SOLUTIONPUSHBACK].begin(), timings_[SOLUTIONPUSHBACK].end()))->second;
        type_time tmax_requestValueUpdate = (std::max_element(timings_[REQUESTVALUEUPDATE].begin(), timings_[REQUESTVALUEUPDATE].end()))->second;
        
    	/*
        out << "=========SUMMARY=========" << "\n";
        out << "# Threads: " << n_threads_ << "\n";
        out << "Total time[s]: " << t_total << " (100%)\n";
        out << "\t- Barriers[s]: " << tt_barrier << "(" << tt_barrier / t_total * 100 << "%)\n";
        out << "\t- Critical at solution.push_back[s]: " << tt_solution_pushback << "(" << tt_solution_pushback / t_total * 100 << "%)\n";
        out << "\t- Critical at child->requestValueUpdate[s]: " << tt_requestValueUpdate << "(" << tt_requestValueUpdate / t_total * 100 << "%) \n";
        
        out << "\n\n";
        
        out << "# Processed nodes (per thread)\n";
        std::for_each(n_processed_nodes.begin(), n_processed_nodes.end(), [&out](size_t& n){out << n << "\t";});
        out << "\n";
    
        out << "Last Syncval before finishing (per thread)\n";
        std::for_each(last_syncVal.begin(), last_syncVal.end(), [&out](size_t& n){out << n << "\t";});
        out << "\n";   
        
        out << "# initial nodes (per thread)\n";
        std::for_each(n_initial_currentnodes.begin(), n_initial_currentnodes.end(), [&out](size_t& n){out << n << "\t";});
        out << "\n";         
        */
    }


};

#else // declare empty inline functions - they will disappear

struct analysis {

	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,N_TIMECAT};

	inline void initialnodes(unsigned tid, unsigned nNodes) { };
	inline void processednodes(unsigned tid, unsigned nNodes) { };
	inline void starttiming(timecat c) { }
	inline void stoptiming(unsigned tid, timecat c) { }

};

#endif // ENABLE_ANALYSIS


// #endif // ANALYSIS_HPP
