#!/bin/bash
#SBATCH --partition=commons
#SBATCH --export=ALL
#SBATCH --job-name=decomp
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=1
#SBATCH --nodes=1 --exclusive
#SBATCH --time=8:00:00
#SBATCH --output=out/slurm-%j.out
#SBATCH --error=err/slurm-%j.err

./decomp benchmarks/$FILENAME.qdimacs > experiments/$FILENAME