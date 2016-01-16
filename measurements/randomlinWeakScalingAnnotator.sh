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
	        str="s/<\/measurement>/\t<comment>deg${DEGREES[$d]}<\/comment>\n\t<\/measurement>/g"
		find ${DIR} -name "*RANDOMLIN*n${nodes}_e${c_edges}*" -print | xargs sed -i $str
	done
done
