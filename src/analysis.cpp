#include "analysis.hpp"


#ifdef ENABLE_ANALYSIS
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <limits.h>
#include <unistd.h>


void analysis::printAnalysis(std::ostream& out){
   
    // TODO: maybe not necessary to extract maximum/average
    //		we can write the timings of all threads to the database instead
    // NOTE: max_element returns pointer to maximum element over all threads
    // second extracts the timing value from the key-value pair

    
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

std::string analysis::suggestBaseFilename(){
    std::string sep = "_";
    #ifdef OPTIMISTIC
    std::string opt = std::to_string(OPTIMISTIC);
    #else
    std::string opt = "0";
    #endif
    
    #ifdef ENABLE_ANALYSIS
    std::string an = std::to_string(ENABLE_ANALYSIS);
    #else
    std::string an = "0";
    #endif

    std::string env_host;
    char hostname[HOST_NAME_MAX];
    int result = gethostname(hostname, HOST_NAME_MAX);
    if (result)
      env_host = "Unk";
    else
      env_host = std::string(hostname);

    
    std::stringstream ss;
           ss << algorithmName_
           << sep << "opt" << opt
           << sep << "an" << an
           << sep << "t" << nThreads_
           << sep << "p" << nProcs_
           << sep << graphName_
           << sep << "n" << nNodes_
           << sep << "e" << nEdges_
           << sep << env_host
           ;
    return ss.str();
}

bool analysis::xmlAnalysis(std::string relativeDir){
    std::stringstream output;
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    output << "<measurements>\n";
    output << "\t<measurement>\n";
    output << "\t\t<date>" << std::time(nullptr) << "</date>\n";
    output << "\t\t<numberOfThreads>" << nThreads_ << "</numberOfThreads>\n";
    output << "\t\t<processors>" << nProcs_ << "</processors>\n";
    output << "\t\t<totalTime>" << time_Total_ << "</totalTime>\n";
    output << "\t\t<algorithm>" << algorithmName_ << "</algorithm>\n";
    output << "\t\t<threads>\n";
    for(size_t i = 0; i < nThreads_; ++i){
        output << "\t\t\t<thread>\n";
        output << "\t\t\t<id>" << i << "</id>\n";
        output << "\t\t\t<timings>\n";
        output << "\t\t\t\t<timing name=\"barrier\">" << timings_[BARRIER][i] << "</timing>\n";
        output << "\t\t\t\t<timing name=\"criticalPushBack\">" << timings_[SOLUTIONPUSHBACK][i] << "</timing>\n";
        output << "\t\t\t\t<timing name=\"criticalRequestValueUpdate\">" << timings_[REQUESTVALUEUPDATE][i] << "</timing>\n";
        output << "\t\t\t</timings>\n";
        output << "\t\t\t<processedNodes>" << count_ProcessedNodes_[i] << "</processedNodes>\n";
        output << "\t\t\t</thread>\n";
    }
    output << "\t\t</threads>\n";
    output << "\t\t<graph>\n";
    output << "\t\t\t<type>" << graphName_ << "</type>\n";
    output << "\t\t\t<numberOfNodes>" << nNodes_ << "</numberOfNodes>\n";
    output << "\t\t\t<numberOfEdges>" << nEdges_ << "</numberOfEdges>\n";
    output << "\t\t</graph>\n";
    #if OPTIMISTIC==1
    output << "\t\t<optimistic>true</optimistic>\n";
    #else
    output << "\t\t<optimistic>false</optimistic>\n";
    #endif
    
    #if ENABLE_ANALYSIS==1
    output << "\t\t<enableAnalysis>true</enableAnalysis>\n";
    #else
    output << "\t\t<enableAnalysis>false</enableAnalysis>\n";
    #endif
    
    output << "\t</measurement>\n";
    output << "</measurements>\n";

    //Check if relativeDir has delimiter
    if(relativeDir == ""){
        relativeDir = "./";
    }
    else if(relativeDir.back() != '/'){
        relativeDir += "/";
    }
    
    std::string baseFilename = relativeDir + suggestBaseFilename();
    std::string filename = baseFilename + ".xml";  
    
    std::random_device rd;
    //std::mt19937 gen(rd());
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(1, 999);

    //Check if file exists 
    while(std::ifstream(filename)){
        std::string rn = std::to_string(dis(gen));
        filename = baseFilename + "." + rn + ".xml";
    }
    std::ofstream f(filename);
    if(f){
        f << output.str();
        f.close();
    }
    else{
        std::cerr << "Could not open file " << filename << std::endl;
        return false;
    }
    
    std::cout << "Analysis written to: " << filename << std::endl;
    return true;
}

#endif
