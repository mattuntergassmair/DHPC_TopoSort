#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "rdtsc_timer.hpp"

#ifdef ENABLE_ANALYSIS
#include <vector>
#include <map>
#include <ostream>

struct analysis {

	// TYPES AND VARIABLES
	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,CURRENTGATHER,CURRENTSCATTER,N_TIMECAT};
	using type_time = double;
	using type_size = unsigned;
	using type_threadcount = short;
	using type_countmap = std::map<type_threadcount , type_size>;
	using type_timingmap = std::map<type_threadcount ,type_time>;
	using type_timingvector = std::vector<type_timingmap>;
	using type_clock = util::rdtsc_timer;
	using type_clockvector = std::vector<type_clock>;

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
	type_time time_Total_;
	type_threadcount nThreads_;
	type_clockvector clocks_;
	type_clock totalclock_;
	type_timingvector timings_;


	// FUNCTIONS
	
	inline void initialnodes(type_threadcount tid, type_size nNodes) {
		count_InitialNodes_[tid] = nNodes;
	}
	
	inline void processednodes(type_threadcount tid, type_size nNodes) {
		count_ProcessedNodes_[tid] = nNodes;
	}
	
	inline void incrementProcessedNodes(type_threadcount tid) { // TODO: check if this can be used instead of processednodes (performance??)
		++count_ProcessedNodes_[tid];
	}

	inline void starttotaltiming();
	
	inline void starttiming(timecat c) {
		clocks_[c].start();
	}
	

	inline void stoptotaltiming();
	
	inline void stoptiming(type_threadcount tid, timecat c) {
		// c stands for the index of the time category we are measuring
		// tid is the thread id
		clocks_[c].stop(); // stop timing
		timings_[c][tid] += clocks_[c].sec(); // get time in seconds and add to total (for given thread)
	}
	
	inline void threadcount(type_threadcount n) {
		nThreads_ = n;
	}


    void printAnalysis(std::ostream& out){
       
       	// TODO: maybe not necessary to extract maximum/average
       	//		we can write the timings of all threads to the database instead
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

	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,CURRENTGATHER,CURRENTSCATTER,N_TIMECAT};
	using type_time = double;
	using type_size = unsigned;
	using type_clock = util::rdtsc_timer;
	using type_threadcount = short;
	
	type_time time_Total_;
	type_clock totalclock_;

	analysis() {}

	inline void initialnodes(type_threadcount tid, type_size nNodes) {}
	inline void processednodes(type_threadcount tid, type_size nNodes) {}
	inline void incrementProcessedNodes(type_threadcount tid) {} // TODO: check if this can be used instead of processednodes (performance??)
	inline void starttotaltiming();
	inline void starttiming(timecat c) {}
	inline void stoptotaltiming();
	inline void stoptiming(type_threadcount tid, timecat c) {}
	inline void threadcount(type_threadcount n) {}
    void printAnalysis(std::ostream& out){}

};

#endif // ENABLE_ANALYSIS


inline void analysis::starttotaltiming() {
	totalclock_.start();
}

inline void analysis::stoptotaltiming() {
	totalclock_.stop();
	time_Total_ = totalclock_.sec();
}


#endif // ANALYSIS_HPP
