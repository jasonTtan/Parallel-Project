ROTATION README
---------------

Directory Structure (Within rotation folder)
   - Makefile is located in rotation root folder
   - Source code and library files are located in src folder
   - Executables are located in bin folder
   - Input files are located in test-images folder

How to Compile
   - In rotation root folder, run "make all" to compile both sequential and parallel executables
   - Makefile uses gcc to compile, as tested on Ubuntu 10.04 and SEAS lnxsrv machines

How to Execute
   - For sequential version: ./bin/rotate-seq [image-file] [rotation degrees]
   - For parallel version: ./bin/rotate-parallel [image-file] [rotation degrees] [nthreads]






