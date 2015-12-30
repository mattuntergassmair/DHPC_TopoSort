nruns=3
if [[ ! -z $1 ]]; then
	nruns=$1
fi

module load boost

export OMP_NUM_THREADS=12
for (( i=0; i<$nruns; i++ ))
do
	bsub -n 12 ./runEulerBarriers.sh 1 12
done

export OMP_NUM_THREADS=24
for (( i=0; i<$nruns; i++ ))
do
	bsub -n 24 ./runEulerBarriers.sh 13 24
done

