module load boost

export OMP_NUM_THREADS=12
for i in {0..2}
do
	bsub -n 12 ./runEulerScalingRandom.sh 1 12
done

export OMP_NUM_THREADS=24
for i in {0..2}
do
	bsub -n 24 ./runEulerScalingRandom.sh 13 24
done

