#!/usr/bin/env bash
# chmod +x run.sh

#==================
# GENERAL SETTINGS
#==================
# Number of threads
THREADS_MIN=$1
THREADS_STEP=1
THREADS_MAX=$2

# Number of repetitions for each experiment
NREP=1

# Executables
declare -a EXE=('./toposort_omp_worksteal_opt2_an0.exe' 
								'./toposort_omp_locallist_opt2_an0.exe'
                './toposort_omp_bitset_opt1_an0.exe'
                './toposort_omp_dynamic_nobarrier_opt2_an0.exe')
SERIAL_EXE='./toposort_serial_opt0_an0.exe'
declare -a GRAPH_TYPES=('s')
RESULTS_DIR='results'

if [ ! -d "$RESULTS_DIR" ]; then
	mkdir "$RESULTS_DIR"
fi

# Problem sizes
N_SSC=1000000    #10^6

#==================================
# STRONG SCALING BENCHMARKS
#==================================
for (( i=0; i<$NREP; i=i+1 ))
do
# Benchmark parallel versions
	for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
	do
		export OMP_NUM_THREADS=$t
		for gt in "${GRAPH_TYPES[@]}"
		do
			for exe in "${EXE[@]}"
			do
				# Strong scaling
				eval "./$exe $gt $N_SSC $RESULTS_DIR"
			done
		done
	done
done


#==================================
# SERIAL REFERENCE 
#==================================

export OMP_NUM_THREADS=1
for gt in "${GRAPH_TYPES[@]}"
do
	eval "./$SERIAL_EXE $gt $N_SSC $RESULTS_DIR"
done
