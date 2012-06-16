all:
	gcc -lm -Ilib lib/libbmp/bmpfile.c -o bin/corner-detection-sequential src/corner-detection-sequential.c 
	gcc -lm -Ilib -fopenmp lib/libbmp/bmpfile.c -o bin/corner-detection-parallel src/corner-detection-parallel.c 

seq:
	gcc -lm -Ilib lib/libbmp/bmpfile.c -o bin/corner-detection-sequential src/corner-detection-sequential.c 

par:
	gcc -lm -Ilib -fopenmp lib/libbmp/bmpfile.c -o bin/corner-detection-parallel src/corner-detection-parallel.c 

clean:
	rm *.bmp
	rm bin/*
