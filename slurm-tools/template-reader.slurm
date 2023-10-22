#!/bin/bash
#SBATCH --nodes=NUMBER_OF_NODES
#SBATCH --mem=MEMORY_REQUIREMENT
#SBATCH --ntasks-per-node=1
#SBATCH --output=output/OUTPUT_DIRECTORY/NUMBER_OF_NODES.txt
#SBATCH --partition=cpu
#SBATCH --account=mzu-delta-cpu
#SBATCH --job-name=reader-test
#SBATCH --time=00:20:00      # hh:mm:ss for the job
#SBATCH --constraint="scratch"

./charmrun ./reader NUMBER_OF_NODES FILESIZE FILEPATH +pNUMBER_OF_NODES
