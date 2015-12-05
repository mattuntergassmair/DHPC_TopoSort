// g++ -fopenmp
#include <iostream>
#include <omp.h>

int main(int argc, char* argv[]){
    #pragma omp parallel
    #pragma omp single
    std::cout << "#Threads: " << omp_get_num_threads() << "\t";
    
    for(int i = 0; i < argc; i++){
        std::cout << "Arg" << i << ": " << argv[i] << "\t";
    }
    std::cout << std::endl;
}
