to compile:
gcc -lm src/bmpfile.c -o bin/scaleSeq src/scale_sequential.c
gcc -lm src/bmpfile.c -fopenmp -o bin/scaleParallel src/scale_parallel.c

to run:
scaleSeq input scaleFactor
scalePar input scaleFactor numThreads
