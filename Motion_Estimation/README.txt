Directory Structure
===================
motion-estimation/
	bin/			contains the compiled executables
	lib/			contains external libraries used for the corner detection code
	src/			contains the source code for the sequential and parallel implementations
	test-files/		contains test images
	Makefile		makefile to compile the sequential and parallel versions of the program	

Compiling
=========
To compile both sequential and parallel versions, run
	make all

To compile just the sequential version, run
	make seq

To compile just the parallel version, run
	make par

To remove all files in the bin folder and bmp's in the motion-detection folder, run
	make clean

You will require a system that has OpenMP installed and results of make will be in the bin folder.

Usage
=====
To execute the program, follow the following usage guidelines below.
sequential
	./bin/motion-estimation-sequential input-file1 input-file2 [blockWidth] [searchPadding]
parallel
	./bin/motion-estimation-parallel input-file1 input-file2 [blockWidth] [searchPadding]

Outputs a file called MotionEstOut.bmp

blockWidth refers to the width of the blocks used in the block-matching algorithm.
searchPadding lets you specify the region around each block that the will be searched to find
a matching block. For example, a searchPadding of 8 will search the
(blockWidth + searchPadding) width block around the current block.

Default values:
blockWidth = 16
searchPadding = 8


