#!/bin/bash

g++ -lm src/EasyBMP.cpp -fopenmp -o bin/projParallel src/proj.cpp
g++ -lm src/EasyBMP.cpp -o bin/projSequential src/projSeq.cpp
