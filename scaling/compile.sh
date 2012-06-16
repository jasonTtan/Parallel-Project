#!/bin/bash

gcc -lm src/bmpfile.c -o bin/scalingSequential src/scale_sequential.c
gcc -lm src/bmpfile.c -fopenmp -o bin/scalingParallel src/scale_parallel.c
