all:
	gcc -lm -Ilib lib/libbmp/bmpfile.c -o bin/motion-estimation-sequential src/Motion_Estimation/motionEstSequential.c

	gcc -lm -Ilib -fopenmp lib/libbmp/bmpfile.c -o bin/motion-estimation-parallel src/Motion_Estimation/motionEstParallel.c

seq:
	gcc -lm -Ilib lib/libbmp/bmpfile.c -o bin/motion-estimation-sequential src/Motion_Estimation/motionEstSequential.c

par:
	gcc -lm -Ilib -fopenmp lib/libbmp/bmpfile.c -o bin/motion-estimation-parallel src/Motion_Estimation/motionEstParallel.c

clean:
	rm *.bmp
	rm bin/*

