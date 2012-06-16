Directory Structure
===================
corner-detection/
	bin/			contains the compiled executables
	lib/			contains external libraries used for the corner detection code
	src/			contains the source code for the sequential and parallel implementations of the corner detection program
	test-files/		contains the images used to test the corner detection program
	Makefile		makefile to compile the sequential and parallel versions of the program	
	time log.txt	log of runs of the sequential and parallel versions on 
gaussianblur/
HPF_Overlay/
Motion_Estimation/
Rotation/
scaling/


Compiling
=========
To compile everything, run
	make all

All external libraries are included and everything has been tested to compile on lnxsrv.


Executables
===========
The main executable, cs133-img-proc.sh can be found at the root. Each function's executable, however, can be found in their respective bin folder.


Input Files
===========
Input files can be found at the root.
	motion-test-input-1.bmp and motion-test-input-2.bmp are used for motion-estimation
	corner-test-red-square.bmp is used for corner-detection
	test-input.bmp is used for all other functions


Usage
=====
To execute the program, run the cs133-img-proc.sh script and follow the following usage guidelines:
	./cs133-img-proc.sh img-op img-op-args
Where
	img-op = { motion-estimation[-parallel], corner-detection[-parallel], rotation[-parallel], scaling[-parallel], gaussian-blur[-parallel], high-pass-filter[-parallel] }
and img-op-args are the arguments for that operation.

You can run each operation without img-op-args arguments like
	./cs133-img-proc.sh corner-detection
to see the usage of that program and the arguments needed for that img-op


Note
====
Any specific details about any image function can be read in the readme in the function's respective folder.