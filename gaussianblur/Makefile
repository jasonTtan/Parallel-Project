all: gb-sequential gb-parallel

gb-sequential: gb-sequential.o EasyBMP.o
	g++ -o ./bin/gb-sequential gb-sequential.o EasyBMP.o
		
gb-parallel: gb-parallel.o EasyBMP.o
	g++ -fopenmp -o ./bin/gb-parallel gb-parallel.o EasyBMP.o

EasyBMP.o: ./src/EasyBMP.cpp ./src/EasyBMP.h ./src/EasyBMP_BMP.h ./src/EasyBMP_DataStructures.h ./src/EasyBMP_VariousBMPutilities.h
	g++ -c ./src/EasyBMP.cpp

gb-sequential.o: ./src/gb-sequential.cpp
	g++ -c ./src/gb-sequential.cpp

gb-parallel.o: ./src/gb-parallel.cpp
	g++ -fopenmp -c ./src/gb-parallel.cpp

clean:
	rm -f ./bin/gb-sequential ./bin/gb-parallel gb-sequential.o gb-parallel.o EasyBMP.o