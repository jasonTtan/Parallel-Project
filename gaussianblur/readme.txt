Directory structure:
	./compile.sh
	./Makefile
	./bin
		./bin/gb-sequential	// executable file of sequential implementation
		./bin/gb-parallel	// executable file of parallel implementation
	./src
		./src/gb-sequential.cpp
		./src/gb-parallel.cpp
		./src/EasyBMP.cpp
		./src/EasyBMP.h
		./src/EasyBMP_BMP.h
		./src/EasyBMP_DataStructures.h
		./src/EasyBMP_VariousBMPutilities.h

How to compile this source code?
compiler: g++
platform: linux

Step-by-step instructions to compile:
$chmod +x compile.sh
$./compile.sh

Where are the executable files located?
In ./bin folder

How to execute this program?

$gb-sequential input_file output_file sigma

$gb-parallel input_file output_file sigma numThreads

