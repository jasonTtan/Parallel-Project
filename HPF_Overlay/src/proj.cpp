#include "EasyBMP.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
using namespace std;

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		wcout << "Correct usage: " << argv[0] << " inputBMP OutputBMP strength numThreads" << endl;
		exit(1);
	}

	timeval startMain,startPar,endMain,endPar;
	double timeDiffMainSeconds, timeDiffMainMilliSeconds, timeDiffMainMicroSeconds, timeDiffParSeconds, timeDiffParMilliSeconds, timeDiffParMicroSeconds;

	gettimeofday(&startMain, NULL);

	int numThreads = atoi( argv[4] );
	omp_set_num_threads(numThreads);

	SetEasyBMPwarningsOff();

	int inputStrength = atoi(argv[3]);	
	double strength = (double)inputStrength/10.0;

	// load in file
	BMP Input,Lower,Result;
	Input.ReadFromFile( argv[1] );
	Lower.ReadFromFile( argv[1] );
	Result.ReadFromFile( argv[1] );

	int width = Input.TellWidth();
	int height = Input.TellHeight();

	gettimeofday(&startPar, NULL);

	// HPF loop
	#pragma omp parallel for schedule(dynamic,32)
	for(int y=0;y<height;y++) // row
	{
		for(int x=0;x<width;x++) // column
		{
			if (x==0 && y==0) // top left
			{
				Input(x,y)->Red = strength * ( (((double)Input(x,y)->Red) * 12) + (((double)Input(x+1,y)->Red) * -2) + (((double)Input(x,y+1)->Red) * -2) + (((double)Input(x+1,y+1)->Red)* -1) );
				Input(x,y)->Green = strength * ( (((double)Input(x,y)->Green) * 12) + (((double)Input(x+1,y)->Green) * -2) + (((double)Input(x,y+1)->Green) * -2) + (((double)Input(x+1,y+1)->Green)* -1) );
				Input(x,y)->Blue = strength * ( (((double)Input(x,y)->Blue) * 12) + (((double)Input(x+1,y)->Blue) * -2) + (((double)Input(x,y+1)->Blue) * -2) + (((double)Input(x+1,y+1)->Blue)* -1) );
			}

			else if(x==width-1 && y==0) // top right
			{
				Input(x,y)->Red = strength * ( (((double)Input(x-1,y)->Red) * -2) + (((double)Input(x,y)->Red) * 12) + (((double)Input(x-1,y+1)->Red) * -1) + (((double)Input(x,y+1)->Red)* -2) );
				Input(x,y)->Green = strength * ( (((double)Input(x-1,y)->Green) * -2) + (((double)Input(x,y)->Green) * 12) + (((double)Input(x-1,y+1)->Green) * -1) + (((double)Input(x,y+1)->Green)* -2) );
				Input(x,y)->Blue = strength * ( (((double)Input(x-1,y)->Blue) * -2) + (((double)Input(x,y)->Blue) * 12) + (((double)Input(x-1,y+1)->Blue) * -1) + (((double)Input(x,y+1)->Blue)* -2) );
			}

			else if(x==0 && y==height-1) // bottom left
			{
				Input(x,y)->Red = strength * ( (((double)Input(x,y-1)->Red) * -2) + (((double)Input(x+1,y-1)->Red) * -1) + (((double)Input(x,y)->Red) * 12) + (((double)Input(x+1,y)->Red) * -2) );
				Input(x,y)->Green = strength * ( (((double)Input(x,y-1)->Green) * -2) + (((double)Input(x+1,y-1)->Green) * -1) + (((double)Input(x,y)->Green) * 12) + (((double)Input(x+1,y)->Green) * -2) );
				Input(x,y)->Blue = strength * ( (((double)Input(x,y-1)->Blue) * -2) + (((double)Input(x+1,y-1)->Blue) * -1) + (((double)Input(x,y)->Blue) * 12) + (((double)Input(x+1,y)->Blue) * -2) );
			}

			else if(x==width-1 && y==height-1) // bottom right
			{
				Input(x,y)->Red = strength * ( (((double)Input(x-1,y-1)->Red) * -1) + (((double)Input(x,y-1)->Red) * -2) + (((double)Input(x-1,y)->Red) * -2) + (((double)Input(x,y)->Red) * 12) );
				Input(x,y)->Green = strength * ( (((double)Input(x-1,y-1)->Green) * -1) + (((double)Input(x,y-1)->Green) * -2) + (((double)Input(x-1,y)->Green) * -2) + (((double)Input(x,y)->Green) * 12) );
				Input(x,y)->Blue = strength * ( (((double)Input(x-1,y-1)->Blue) * -1) + (((double)Input(x,y-1)->Blue) * -2) + (((double)Input(x-1,y)->Blue) * -2) + (((double)Input(x,y)->Blue) * 12) );
			}
			
			else if(x==0) // left edge
			{
				Input(x,y)->Red = strength * ( (((double)Input(x,y-1)->Red) * -2) + (((double)Input(x+1,y-1)->Red) * -1) + (((double)Input(x,y)->Red) * 12) + (((double)Input(x+1,y)->Red) * -2) + (((double)Input(x,y+1)->Red) * -2) + (((double)Input(x+1,y+1)->Red) * -1) );
				Input(x,y)->Green = strength * ( (((double)Input(x,y-1)->Green) * -2) + (((double)Input(x+1,y-1)->Green) * -1) + (((double)Input(x,y)->Green) * 12) + (((double)Input(x+1,y)->Green) * -2) + (((double)Input(x,y+1)->Green) * -2) + (((double)Input(x+1,y+1)->Green) * -1) );
				Input(x,y)->Blue = strength * ( (((double)Input(x,y-1)->Blue) * -2) + (((double)Input(x+1,y-1)->Blue) * -1) + (((double)Input(x,y)->Blue) * 12) + (((double)Input(x+1,y)->Blue) * -2) + (((double)Input(x,y+1)->Blue) * -2) + (((double)Input(x+1,y+1)->Blue) * -1) );
			}

			else if(y==0) // top edge
			{
				Input(x,y)->Red = strength * ( (((double)Input(x-1,y)->Red) * -2) + (((double)Input(x,y)->Red) * 12) + (((double)Input(x+1,y)->Red) * -2) + (((double)Input(x-1,y+1)->Red) * -1) + (((double)Input(x,y+1)->Red) * -2) + (((double)Input(x+1,y+1)->Red) * -1) );
				Input(x,y)->Green = strength * ( (((double)Input(x-1,y)->Green) * -2) + (((double)Input(x,y)->Green) * 12) + (((double)Input(x+1,y)->Green) * -2) + (((double)Input(x-1,y+1)->Green) * -1) + (((double)Input(x,y+1)->Green) * -2) + (((double)Input(x+1,y+1)->Green) * -1) );
				Input(x,y)->Blue = strength * ( (((double)Input(x-1,y)->Blue) * -2) + (((double)Input(x,y)->Blue) * 12) + (((double)Input(x+1,y)->Blue) * -2) + (((double)Input(x-1,y+1)->Blue) * -1) + (((double)Input(x,y+1)->Blue) * -2) + (((double)Input(x+1,y+1)->Blue) * -1) );
			}

			else if(x==width-1) // right edge
			{
				Input(x,y)->Red = strength * ( (((double)Input(x-1,y-1)->Red) * -1) + (((double)Input(x,y-1)->Red) * -2) + (((double)Input(x-1,y)->Red) * -2) + (((double)Input(x,y)->Red) * 12) + (((double)Input(x-1,y+1)->Red) * -1) + (((double)Input(x,y+1)->Red) * -2) );
				Input(x,y)->Green = strength * ( (((double)Input(x-1,y-1)->Green) * -1) + (((double)Input(x,y-1)->Green) * -2) + (((double)Input(x-1,y)->Green) * -2) + (((double)Input(x,y)->Green) * 12) + (((double)Input(x-1,y+1)->Green) * -1) + (((double)Input(x,y+1)->Green) * -2) );
				Input(x,y)->Blue = strength * ( (((double)Input(x-1,y-1)->Blue) * -1) + (((double)Input(x,y-1)->Blue) * -2) + (((double)Input(x-1,y)->Blue) * -2) + (((double)Input(x,y)->Blue) * 12) + (((double)Input(x-1,y+1)->Blue) * -1) + (((double)Input(x,y+1)->Blue) * -2) );
			}

			else if(y==height-1) // bottom edge
			{
				Input(x,y)->Red = strength * ( (((double)Input(x-1,y-1)->Red) * -1) + (((double)Input(x,y-1)->Red) * -2) + (((double)Input(x+1,y-1)->Red) * -1) + (((double)Input(x-1,y)->Red) * -2) + (((double)Input(x,y)->Red) * 12) + (((double)Input(x+1,y)->Red) * -2) );
				Input(x,y)->Green = strength * ( (((double)Input(x-1,y-1)->Green) * -1) + (((double)Input(x,y-1)->Green) * -2) + (((double)Input(x+1,y-1)->Green) * -1) + (((double)Input(x-1,y)->Green) * -2) + (((double)Input(x,y)->Green) * 12) + (((double)Input(x+1,y)->Green) * -2) );
				Input(x,y)->Blue = strength * ( (((double)Input(x-1,y-1)->Blue) * -1) + (((double)Input(x,y-1)->Blue) * -2) + (((double)Input(x+1,y-1)->Blue) * -1) + (((double)Input(x-1,y)->Blue) * -2) + (((double)Input(x,y)->Blue) * 12) + (((double)Input(x+1,y)->Blue) * -2) );
			}

			else if (x>0 && x<width-1 && y>0 && y<height-1) // everything else
			{
				// apply kernel red
				Input(x,y)->Red = strength * ( (((double)Input(x-1,y-1)->Red) * -1) + (((double)Input(x,y-1)->Red) * -2) + (((double)Input(x+1,y-1)->Red) * -1) + (((double)Input(x-1,y)->Red) * -2) + (((double)Input(x,y)->Red) *12) + (((double)Input(x+1,y)->Red) * -2) + (((double)Input(x-1,y+1)->Red) * -1) + (((double)Input(x,y+1)->Red) * -2) + (((double)Input(x+1,y+1)->Red) * -1) );
				// apply kernel green
				Input(x,y)->Green = strength * ( (((double)Input(x-1,y-1)->Green) * -1) + (((double)Input(x,y-1)->Green) * -2) + (((double)Input(x+1,y-1)->Green) * -1) + (((double)Input(x-1,y)->Green) * -2) + (((double)Input(x,y)->Green) *12) + (((double)Input(x+1,y)->Green) * -2) + (((double)Input(x-1,y+1)->Green) * -1) + (((double)Input(x,y+1)->Green) * -2) + (((double)Input(x+1,y+1)->Green) * -1) );
				// apply kernel blue
				Input(x,y)->Blue = strength * ( (((double)Input(x-1,y-1)->Blue) * -1) + (((double)Input(x,y-1)->Blue) * -2) + (((double)Input(x+1,y-1)->Blue) * -1) + (((double)Input(x-1,y)->Blue) * -2) + (((double)Input(x,y)->Blue) *12) + (((double)Input(x+1,y)->Blue) * -2) + (((double)Input(x-1,y+1)->Blue) * -1) + (((double)Input(x,y+1)->Blue) * -2) + (((double)Input(x+1,y+1)->Blue) * -1) );
			}
		}
	}
	
	// overlay loop	
	#pragma omp parallel for schedule(dynamic,32)
	for(int y=0;y<height;y++) // row
	{
		for(int x=0;x<width;x++) // column
		{
			double valueUnit, minValue;
			
			if ((double) Lower(x,y)->Red > 127.5)
			{
				valueUnit = (255 - (double)Lower(x,y)->Red)/127.5;
				minValue = (double)Lower(x,y)->Red - (255 - (double)Lower(x,y)->Red);
				Result(x,y)->Red = ((double)Input(x,y)->Red * valueUnit) + minValue;
			}
			else
			{
				valueUnit = (double)Lower(x,y)->Red/127.5;
				Result(x,y)->Red = ((double)Input(x,y)->Red * valueUnit);
			}
			
			if ((double) Lower(x,y)->Green > 127.5)
			{
				valueUnit = (255 - (double)Lower(x,y)->Green)/127.5;
				minValue = (double)Lower(x,y)->Green - (255 - (double)Lower(x,y)->Green);
				Result(x,y)->Green = ((double)Input(x,y)->Green * valueUnit) + minValue;
			}
			else
			{
				valueUnit = (double)Lower(x,y)->Green/127.5;
				Result(x,y)->Green = ((double)Input(x,y)->Green * valueUnit);
			}	
			
			if ((double) Lower(x,y)->Blue > 127.5)
			{
				valueUnit = (255 - (double)Lower(x,y)->Blue)/127.5;
				minValue = (double)Lower(x,y)->Blue - (255 - (double)Lower(x,y)->Blue);
				Result(x,y)->Blue = ((double)Input(x,y)->Blue * valueUnit) + minValue;
			}
			else
			{
				valueUnit = (double)Lower(x,y)->Blue/127.5;
				Result(x,y)->Blue = ((double)Input(x,y)->Blue * valueUnit);
			}
		}
	}

	gettimeofday(&endPar, NULL);

	timeDiffParSeconds = endPar.tv_sec - startPar.tv_sec;
	timeDiffParMicroSeconds = endPar.tv_usec - startPar.tv_usec;
	timeDiffParMilliSeconds = ((timeDiffParSeconds) * 1000 + timeDiffParMicroSeconds/1000.0) + 0.5;

	cout << "The parallel section took (seconds): " << timeDiffParSeconds << endl;
	cout << "The parallel section took (milliseconds): " << timeDiffParMilliSeconds << endl;
	cout << "The parallel section took (microseconds): " << timeDiffParMicroSeconds << endl;

	Result.WriteToFile( argv[2] );	
	
	gettimeofday(&endMain, NULL);

	timeDiffMainSeconds = endMain.tv_sec - startMain.tv_sec;
	timeDiffMainMicroSeconds = endMain.tv_usec - startMain.tv_usec;
	timeDiffMainMilliSeconds = ((timeDiffMainSeconds) * 1000 + timeDiffMainMicroSeconds/1000.0) + 0.5;

	cout << "The main program took (seconds): " << timeDiffMainSeconds << endl;
	cout << "The main program took (milliseconds): " << timeDiffMainMilliSeconds << endl;
	cout << "The main program took (microseconds): " << timeDiffMainMicroSeconds << endl;



}
