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
NREP=1

# Executables
SERIAL_EXE='./toposort_serial.exe'
declare -a GRAPH_TYPES=('r' 's')
#SERIAL_EXE='dryrun/test.exe'

# Problem sizes
N_SSC=100000000    #10^8
N_WSC=10000000  #10^7

ANALYSIS=0
for (( i=0; i<$NREP; i=i+1 ))
do
	# Benchmark serial version
	export OMP_NUM_THREADS=1
	make clean
	make -j release OPT=0 AN=$ANALYSIS

	for gt in "${GRAPH_TYPES[@]}"
	do
		# Strong scaling
		eval "./$SERIAL_EXE $gt $N_SSC"
		# Weak scaling
		for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
		do
			ws=$(($N_WSC*$t))
			eval "./$SERIAL_EXE $gt $ws"
		done
	done
done
