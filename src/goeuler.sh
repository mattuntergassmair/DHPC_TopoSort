module load boost

export OMP_NUM_THREADS=12
bsub -n 12 ./runEulerParallel.sh 1 12

export OMP_NUM_THREADS=24
bsub -n 24 ./runEulerParallel.sh 12 24

export OMP_NUM_THREADS=12
bsub -n 12 ./runEulerPercentage.sh 1 12

export OMP_NUM_THREADS=24
bsub -n 24 ./runEulerPercentage.sh 12 24

export OMP_NUM_THREADS=1
bsub -n 1 ./runEulerSerial.sh
