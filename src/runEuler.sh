#!/usr/bin/env bash
# chmod +x run.sh
SIZE_MIN=5000
SIZE_STEP=10000
SIZE_MAX=105000

THREADS_MIN=1
THREADS_STEP=1
THREADS_MAX=24

NREP=12

ANALYSIS=0

declare -a EXE=('./toposort_omp_bitset.exe' './toposort_omp_locallist.exe');
SERIALEXE='./toposort_serial.exe'

#declare -a EXE=('dryrun/test.exe')
#SERIALEXE='dryrun/test.exe'

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
			for (( s=$SIZE_MIN; s<=$SIZE_MAX; s=s+$SIZE_STEP ))
			do
				for v in "${EXE[@]}"
				do
					eval "./$v $s"
				done
			done     
		done
	done

	# Benchmark serial version
	export OMP_NUM_THREADS=1
	for op in {0..1}
	do
		make clean
		make -j release OPT=$op AN=$ANALYSIS
		for (( s=$SIZE_MIN; s<=$SIZE_MAX; s=s+$SIZE_STEP ))
		do
			eval "./$SERIALEXE $s"
		done     
	done
done
