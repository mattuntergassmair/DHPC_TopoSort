#!/usr/bin/env bash
# chmod +x run.sh

#==================
# GENERAL SETTINGS
#==================
# Number of threads
THREADS_MIN=1
THREADS_STEP=1
THREADS_MAX=24

# Number of repetitions for each experiment
NREP=3

# Executables
declare -a PARALLEL_EXE=('./toposort_omp_bitset.exe' './toposort_omp_locallist.exe');
SERIAL_EXE='./toposort_serial.exe'
declare -a GRAPH_TYPES=('r' 's')
#declare -a PARALLEL_EXE=('dryrun/test.exe')
#SERIAL_EXE='dryrun/test.exe'

# Problem sizes
N_SSC=1000000000    #10^9
N_WSC=100000000  #10^8


#==================================
# STRONG / WEAK SCALING BENCHMARKS
#==================================
ANALYSIS=0

for (( i=0; i<$NREP; i=i+1 ))
do
	# Benchmark parallel versions
	for op in {0..1}
	do
		make clean
		make -j release OPT=$op AN=$ANALYSIS
		for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
		do
			export OMP_NUM_THREADS=$t
			for v in "${PARALLEL_EXE[@]}"
			do
				for gt in "${GRAPH_TYPES[@]}"
				do
					# Strong scaling
					eval "./$v $gt $N_SSC"
					# Weak scaling
					ws=$(($N_WSC*$t))
					eval "./$v $gt $ws"
				done
			done    
		done
	done


	# Benchmark serial version
	export OMP_NUM_THREADS=1
	make clean
	make -j release OPT=$op AN=$ANALYSIS

	for gt in "${GRAPH_TYPES[@]}"
	do
		# Strong scaling
		eval "./$v $gt $N_SSC"
		# Weak scaling
		for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
		do
			ws=$(($N_WSC*$t))
			eval "./$v $gt $ws"
		done
	done
done


#=======================
# PERCENTAGE BENCHMARKS
#=======================
ANALYSIS=1
for (( i=0; i<$NREP; i=i+1 ))
do
	# Benchmark parallel versions
	for op in {0..1}
	do
		make clean
		make -j release OPT=$op AN=$ANALYSIS
		for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
		do
			export OMP_NUM_THREADS=$t
			for v in "${PARALLEL_EXE[@]}"
			do
				for gt in "${GRAPH_TYPES[@]}"
				do
					# Strong scaling
					eval "./$v $gt $N_SSC"
				done
			done    
		done
	done
done


