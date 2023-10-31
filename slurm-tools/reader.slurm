#!/bin/bash
#SBATCH --nodes=16
#SBATCH --tasks-per-node=NTASKS
#SBATCH --mem=16g
#SBATCH --output=output/non-smp/16-nodes-64GB-exclusive/NTASKStask-runRUN_NUMBER.txt
#SBATCH --partition=cpu
#SBATCH --account=mzu-delta-cpu
#SBATCH --job-name=reader-test
#SBATCH --time=00:20:00      # hh:mm:ss for the job
#SBATCH --constraint="scratch"
#SBATCH --exclusive
 
./charmrun ./reader NCHARES 64 /scratch/mzu/mtaylor2/clearCache.txt +pNCHARES
./charmrun ./reader NCHARES 64 /scratch/mzu/mtaylor2/64GB.txt +pNCHARES

