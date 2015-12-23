#!/usr/bin/env bash
# chmod +x run.sh

module load boost
for opt in {0..2}
do
# Benchmark parallel versions
	for an in {0..1}
	do
		make clean	
		make -j release OPT=$opt AN=$an
	done
done


