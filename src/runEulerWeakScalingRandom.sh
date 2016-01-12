#!/usr/bin/env bash
# chmod +x run.sh

#==================
# GENERAL SETTINGS
#==================
# Number of threads
THREADS_MIN=$1
THREADS_STEP=1
THREADS_MAX=$2 #inclusive

# Number of repetitions for each experiment
NREP=3

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

# Weak scaling base size
N_WSC=100000

#==================================
# STRONG SCALING BENCHMARKS
#==================================
for (( i=0; i<$NREP; i=i+1 ))
do
	for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+1 ))
  do
		export OMP_NUM_THREADS=$t
		for gt in "${GRAPH_TYPES[@]}"
		do
			for exe in "${EXE[@]}"
			do
				for nd in "${NODE_DEGREES[@]}"
				do
					# Strong scaling
          wsc2=$(( $N_WSC*$t ))
					eval "./$exe $gt $wsc2 $RESULTS_DIR $nd"
				done
			done
		done
  done
done


#==================================
# SERIAL REFERENCE 
#==================================
export OMP_NUM_THREADS=1
for (( i=0; i<$NREP; i=i+1 ))
do
	for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+1 ))
  do
		for gt in "${GRAPH_TYPES[@]}"
		do
			for nd in "${NODE_DEGREES[@]}"
			do
				# Strong scaling
        wsc=$(( $N_WSC*$t ))
				eval "./$SERIAL_EXE $gt $wsc $RESULTS_DIR $nd"
			done
		done
  done
done
