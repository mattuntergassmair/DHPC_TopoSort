#!/usr/bin/env bash
#SBATCH --gres=mic:1 -n 1 -N 1

export SINK_LD_LIBRARY_PATH=/opt/intel/composer_xe_2015/lib/mic/

#==================
# GENERAL SETTINGS
#==================

OUT_DIR='results/'

# Number of threads
THREADS_MIN=1
THREADS_STEP=1
THREADS_MAX=60

# Number of repetitions for each experiment
NREP=3

# Random number to make files distinguishable
RN=$(shuf -i 0-99999 -n 1)
HOSTNAME='xeonPhi'

# Executables and verbose names
declare -a PARALLEL_EXE=('./toposort_omp_bitset.exe' './toposort_omp_locallist.exe')
declare -a PARALLEL_EXE_NAME=('bitset' 'locallist')
SERIAL_EXE='./toposort_serial.exe'
SERIAL_EXE_NAME='serial'
declare -a GRAPH_TYPES=('r' 's')
declare -a GRAPH_TYPES_NAME=('RANDOM' 'SOFTWARE')

#declare -a PARALLEL_EXE=('dryrun/test.exe')
#SERIAL_EXE='dryrun/test.exe'

# Problem sizes
N_SSC=10000000  #10^7
N_WSC=10000000  #10^7


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
			for ((v=0; v < ${#PARALLEL_EXE[*]}; v++ ))
			do
				for ((gt=0; gt < ${#GRAPH_TYPES[*]}; gt++ ))
				do
					# Strong scaling
					filename_ssc="$OUT_DIR${PARALLEL_EXE_NAME[$v]}_opt${op}_an${ANALYSIS}_t${t}_p${THREADS_MAX}_${GRAPH_TYPES_NAME[$gt]}_n${N_SSC}_${HOSTNAME}.$RN$i.xml"
					eval "srun --gres=mic:1 --output=$filename_ssc micnativeloadex ./${PARALLEL_EXE[$v]} ${GRAPH_TYPES[$gt]} $N_SSC -e KMP_PLACE_THREADS=${t}c,1t"
					# Weak scaling
					ws=$(($N_WSC*$t))
					filename_wsc="$OUT_DIR${PARALLEL_EXE_NAME[$v]}_opt${op}_an${ANALYSIS}_t${t}_p${THREADS_MAX}_${GRAPH_TYPES_NAME[$gt]}_n${ws}_${HOSTNAME}.$RN$i.xml"
					eval "srun --gres=mic:1 --output=$filename_wsc micnativeloadex ./${PARALLEL_EXE[$v]} ${GRAPH_TYPES[$gt]} $ws -e KMP_PLACE_THREADS=${t}c,1t"
				done
			done    
		done
	done


	# Benchmark serial version
	for op in {0..1}
	do
		make clean
		make -j release OPT=$op AN=$ANALYSIS

		for ((gt=0; gt < ${#GRAPH_TYPES[*]}; gt++ ))
		do
			# Strong scaling
			filename_ssc="$OUT_DIR${SERIAL_EXE_NAME}_opt${op}_an${ANALYSIS}_t1_p1_${GRAPH_TYPES_NAME[$gt]}_n${N_SSC}_${HOSTNAME}.$RN$i.xml"
			eval "srun --gres=mic:1 --output=$filename_ssc micnativeloadex ./${SERIAL_EXE} ${GRAPH_TYPES[$gt]} $N_SSC -e KMP_PLACE_THREADS=1c,1t"
			# Weak scaling
			for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
			do
				ws=$(($N_WSC*$t))
				filename_wsc="$OUT_DIR${SERIAL_EXE_NAME}_opt${op}_an${ANALYSIS}_t1_p1_${GRAPH_TYPES_NAME[$gt]}_n${ws}_${HOSTNAME}.$RN$i.xml"
				eval "srun --gres=mic:1 --output=$filename_wsc micnativeloadex ./${SERIAL_EXE} ${GRAPH_TYPES[$gt]} $ws -e KMP_PLACE_THREADS=1c,1t"
			done
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
			for ((v=0; v < ${#PARALLEL_EXE[*]}; v++ ))
			do
				for ((gt=0; gt < ${#GRAPH_TYPES[*]}; gt++ ))
				do
					# Strong scaling
					filename_ssc="$OUT_DIR${PARALLEL_EXE_NAME[$v]}_opt${op}_an${ANALYSIS}_t${t}_p${THREADS_MAX}_${GRAPH_TYPES_NAME[$gt]}_n${N_SSC}_${HOSTNAME}.$RN$i.xml"
					eval "srun --gres=mic:1 --output=$filename_ssc micnativeloadex ./${PARALLEL_EXE[$v]} ${GRAPH_TYPES[$gt]} $N_SSC -e KMP_PLACE_THREADS=${t}c,1t"
				done
			done    
		done
	done
done


