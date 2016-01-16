#!/bin/bash

declare -a EDGES=( 799933 1599743 3198981 6395815 )
declare -a DEGREES=( 8 16 32 64 )

THREADS_MIN=1
THREADS_MAX=24
DIR="dataWeakScalingAllWithoutSoftwareTest"

for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+1 ))
do
	for (( d=0; d<4; d=d+1 ))
	do
		c_edges=$(( ${EDGES[$d]}*$t/100000 ))
		nodes=$(( 100000*$t ))
		str="s/RANDOMLIN/RANDOMLIN${DEGREES[$d]}/g"
		fname="*RANDOMLIN*n${nodes}_e${c_edges}*"
                rename $str ${DIR}/$fname
		find ${DIR} -name "$str" -print | xargs sed -i $str
	done
done
