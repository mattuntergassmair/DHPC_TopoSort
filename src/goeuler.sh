export OMP_NUM_THREADS=24
bsub -W 00:20 -n 24 ./runEuler.sh
