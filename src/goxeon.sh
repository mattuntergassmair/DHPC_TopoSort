#!/bin/sh
#SBATCH --time=00:01:00
#SBATCH --gres=mic:1 -n 1 -N 1



export SINK_LD_LIBRARY_PATH=/opt/intel/composer_xe_2015/lib/mic/
srun --gres=mic:1 micnativeloadex ./toposort_omp_bitset.exe
