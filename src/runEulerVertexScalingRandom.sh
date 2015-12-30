#!/usr/bin/env bash
# chmod +x run.sh

#==================
# GENERAL SETTINGS
#==================
# Number of threads
N_THREADS=$1

# Number of repetitions for each experiment
NREP=1

# Executables
declare -a EXE=('./toposort_omp_worksteal_opt2_an0.exe' 
                './toposort_omp_bitset_opt1_an0.exe'
                './toposort_omp_dynamic_nobarrier_opt2_an0.exe')
SERIAL_EXE='./toposort_serial_opt0_an0.exe'
declare -a GRAPH_TYPES=('r')
declare -a NODE_DEGREES=(8 16 32 64)
RESULTS_DIR='results'

if [ ! -d "$RESULTS_DIR" ]; then
	mkdir "$RESULTS_DIR"
fi

# Problem sizes
declare -a N_VSC=(100000 200000 400000 800000 1600000 3200000)

#==================================
# STRONG SCALING BENCHMARKS
#==================================
for (( i=0; i<$NREP; i=i+1 ))
do
# Benchmark parallel versions
	export OMP_NUM_THREADS=$N_THREADS
	for gt in "${GRAPH_TYPES[@]}"
	do
		for exe in "${EXE[@]}"
		do
			for nvsc in "${N_VSC[@]}"
			do
				for nd in "${NODE_DEGREES[@]}"
				do
					# Strong scaling
					eval "./$exe $gt $nvsc $RESULTS_DIR $nd"
				done
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
	for nvsc in "${N_VSC[@]}"
	do
		for nd in "${NODE_DEGREES[@]}"
		do
			eval "./$SERIAL_EXE $gt $nvsc $RESULTS_DIR $nd"
		done
	done
done
