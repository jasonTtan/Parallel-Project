#include "EasyBMP.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
using namespace std;

int main(int argc, char **argv)
{
	timeval start,end;

	double seconds, mseconds, useconds;

	gettimeofday(&start, NULL);
	usleep(2000);
	gettimeofday(&end, NULL);

	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;

	mseconds = ((seconds) * 10000 + useconds/1000.0) + 0.5;

	cout << "Elapsed time (seconds): " << seconds << endl;
	cout << "Elapsed time (milliseconds): " << mseconds << endl;
	cout << "Elapsed time (microseconds): " << useconds << endl;
}
