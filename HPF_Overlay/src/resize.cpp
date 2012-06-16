#include "EasyBMP.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv)
{
	BMP AnImage;
	AnImage.ReadFromFile( argv[1] );

	// rescale to input size %
	Rescale( AnImage , 'p', atoi(argv[3]) );
	
	// write to output
	AnImage.WriteToFile( argv[2] );
}
