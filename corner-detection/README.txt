Directory Structure
===================
corner-detection/
	bin/			contains the compiled executables
	lib/			contains external libraries used for the corner detection code
	src/			contains the source code for the sequential and parallel implementations of the corner detection program
	test-files/		contains the images used to test the corner detection program
	Makefile		makefile to compile the sequential and parallel versions of the program	
	time log.txt	log of runs of the sequential and parallel versions on images


Compiling
=========
To compile both sequential and parallel versions, run
	make all

To compile just the sequential version, run
	make seq

To compile just the parallel version, run
	make par

To remove all files in the bin folder and bmp's in the corner-detection folder, run
	make clean

You will require a system that has OpenMP installed and results of make will be in the bin folder.


Usage
=====
To execute the program, follow the following usage guidelines
	sequential
		./bin/corner-detection-sequential input-file output-file [threshold]
	parallel
		./bin/corner-detection-parallel input-file output-file [threshold] [num_threads]