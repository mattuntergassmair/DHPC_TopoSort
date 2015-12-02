#!/usr/bin/env bash
# chmod +x run.sh
SIZE_MIN=5000
SIZE_STEP=10000
SIZE_MAX=30000

THREADS_MIN=1
THREADS_STEP=1
THREADS_MAX=4

ANALYSIS=0

declare -a EXE=('./toposort_omp_bitset.exe' './toposort_omp_locallist.exe');

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
