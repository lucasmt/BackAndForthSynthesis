#!/bin/bash

for FILE in benchmarks/*.qdimacs
do
	export FILENAME=$(basename ${FILE//.qdimacs/})
	sbatch decomp.slurm
done
