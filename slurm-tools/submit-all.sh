#!/bin/bash

filepath=$1
outdir=$2
filesize=$3

sed -e "s|OUTPUT_DIRECTORY|$outdir|g" -e "s|FILESIZE|$filesize|g" -e "s|FILEPATH|$filepath|g" slurm-tools/template-reader.slurm > slurm-tools/reader-o.slurm
for i in 1 2 4 8 16 32 48 64 72 80 88 96 104 112
do
    mem=$(expr $filesize / $i + 2)
    unit="g"
    m="$mem$unit"
    sed -e "s|MEMORY_REQUIREMENT|$m|g" -e "s|NUMBER_OF_NODES|$i|g" slurm-tools/reader-o.slurm > slurm-tools/reader-$i.slurm   
    sbatch slurm-tools/reader-$i.slurm
done


