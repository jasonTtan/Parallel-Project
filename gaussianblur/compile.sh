#!/bin/sh

echo g++ -o ./bin/gb-sequential ./src/gb-sequential.cpp ./src/EasyBMP.cpp
g++ -o ./bin/gb-sequential ./src/gb-sequential.cpp ./src/EasyBMP.cpp

echo g++ -fopenmp -o ./bin/gb-parallel ./src/gb-parallel.cpp ./src/EasyBMP.cpp
g++ -fopenmp -o ./bin/gb-parallel ./src/gb-parallel.cpp ./src/EasyBMP.cpp
