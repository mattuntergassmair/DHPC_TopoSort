nruns=6
if [[ ! -z $1 ]]; then
	nruns=$1
fi

module load boost

export OMP_NUM_THREADS=12
for (( i=0; i<$nruns; i++ ))
do
	bsub -n 12 ./runEulerWeakScalingSoftware.sh 1 12
done

export OMP_NUM_THREADS=24
for (( i=0; i<$nruns; i++ ))
do
	bsub -n 24 ./runEulerWeakScalingSoftware.sh 13 24
done
