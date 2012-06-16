#include "EasyBMP.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		wcout << "Correct usage: " << argv[0] << " inputBMP OutputBMP strength" << endl;
		exit(1);
	}

	time_t startMain,startPar,endMain,endPar;
	double timeDiffMain,timeDiffPar;
	time (&startMain);

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

	time (&startPar);

	// HPF loop
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
	/*	
	// overlay loop	
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
	*/

	time(&endPar);

	timeDiffPar = difftime(endPar,startPar);

	cout << "The parallel section took: " << timeDiffPar << " secs" << endl;

	Input.WriteToFile( argv[2] );	
	
	time(&endMain);

	timeDiffMain = difftime(endMain,startMain);

	cout << "The main program took: " << timeDiffMain << " secs" << endl;


}
