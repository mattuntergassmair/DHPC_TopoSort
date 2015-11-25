#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <vector>
#include <ostream>
#include <omp.h>

struct analysis{
    using time_t = double;
    analysis()

    {
        #pragma omp parallel
        {
            #pragma omp single
            n_threads_ = omp_get_num_threads();
        }
      n_processed_nodes = std::vector<size_t>(n_threads_, 0);
      last_syncVal = std::vector<size_t>(n_threads_, 0);
      n_initial_currentnodes  = std::vector<size_t>(n_threads_, 0);
      t_barrier = std::vector<time_t>(n_threads_, 0.);
      t_solution_pushback = std::vector<time_t>(n_threads_, 0.);
      t_requestValueUpdate = std::vector<time_t>(n_threads_, 0.);
      t_total = 0.;
    }
    
    int n_threads_;
    std::vector<size_t> n_processed_nodes;
    std::vector<size_t> last_syncVal;
    std::vector<size_t> n_initial_currentnodes;
    
    time_t t_total;
    time_t t_timing_overhead;
    std::vector<time_t> t_barrier;
    std::vector<time_t> t_solution_pushback;
    std::vector<time_t> t_requestValueUpdate;
    
    void summary(std::ostream& out){
        time_t tt_barrier = *(std::max_element(t_barrier.begin(), t_barrier.end()));
        time_t tt_solution_pushback = *(std::max_element(t_solution_pushback.begin(), t_solution_pushback.end()));
        time_t tt_requestValueUpdate = *(std::max_element(t_requestValueUpdate.begin(), t_requestValueUpdate.end()));
        
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
    }
};


#endif
