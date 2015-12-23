module load boost

export OMP_NUM_THREADS=12
for i in {0..2}
do
	bsub -n 12 ./runEulerVertexScalingRandom.sh 12
done
