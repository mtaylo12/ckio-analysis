#!/bin/bash

for tasks in {81..100..4}
do
    chares=$(($tasks*16)) 
    sed -e "s|NTASKS|$tasks|g" -e "s|NCHARES|$chares|g" slurm-tools/reader.slurm > slurm-tools/reader1.slurm

    for i in {1..10}
    do
	sed -e "s|RUN_NUMBER|$i|g" slurm-tools/reader1.slurm > slurm-tools/reader2.slurm
	sbatch slurm-tools/reader2.slurm
	
    done
done



