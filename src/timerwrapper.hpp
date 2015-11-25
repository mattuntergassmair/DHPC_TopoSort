#ifdef ENABLE_ANALYSIS
#include "rdtsc_timer.hpp"
#endif

#include "analysis.hpp"


#ifdef ENABLE_ANALYSIS
struct timerwrapper {

	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,N_TIMECAT};

	/*
	util::rdtsc_timer rt_barrier;
	util::rdtsc_timer rt_solution_pushback;
	util::rdtsc_timer rt_requestValueUpdate;
	*/
	// <==>
	std::vector<util::rdtsc_timer> timers; // (10); // N_TIMECAT); // TODO: check

	analysis analysis_;

	inline void initialnodes(unsigned tid, unsigned nNodes) {
		analysis_.n_initial_currentnodes[tid] = nNodes;
	};
	
	inline void processednodes(unsigned tid, unsigned nNodes) {
		analysis_.n_processed_nodes[tid] = nNodes;
	};

	inline void starttiming(timecat c) {
		timers[c].start();
	}

	inline void stoptiming(unsigned tid, timecat c) {
		timers[c].stop();
		switch(c) {
			case BARRIER:
				analysis_.t_barrier[tid] += timers[c].sec(); // TODO: make generic by changing analysis
				break;
			case SOLUTIONPUSHBACK:
				analysis_.t_solution_pushback[tid] += timers[c].sec();
				break;
			case REQUESTVALUEUPDATE:
				analysis_.t_requestValueUpdate[tid] += timers[c].sec();
				break;
			default:
				std::cout << "\n\nERROR: Timecat not registered\n\n";
				break;
		}
	}

};

#else // declare empty inline functions - they will disappear

struct timerwrapper {

	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,N_TIMECAT};

	inline void initialnodes(unsigned tid, unsigned nNodes) { };
	inline void processednodes(unsigned tid, unsigned nNodes) { };
	inline void starttiming(timecat c) { }
	inline void stoptiming(unsigned tid, timecat c) { }

};

#endif // ENABLE_ANALYSIS






