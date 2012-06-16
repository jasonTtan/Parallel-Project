#include "EasyBMP.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	// load in file
	BMP Input;
	Input.ReadFromFile( argv[1] );
	cout << "File info:" << endl;
	cout << "Width x Height: " << Input.TellWidth() << " x " << Input.TellHeight() << " at " << Input.TellBitDepth() << " bpp" << endl;
	cout << "Colors: " << Input.TellNumberOfColors() << endl;

	Input.SetBitDepth(8);
	if(argc == 3)
	{
		Input.WriteToFile( argv[2] );
		return 0;
	}
	/*

	// show the color of pixel (14,18)
	cout << "(Red:" << (int) Input(14,18)->Red << ", "
		<< "Green: " << (int) Input(14,18)->Green << ", "
		<< "Blue: " << (int) Input(14,18)->Blue << ", "
		<< "Alpha: " << (int) Input(14,18)->Alpha << ")" << endl;

	// color table testing
	cout << "color table testing, 255th color info" << endl;
	RGBApixel someColor = Input.GetColor(255);
	// Display the color
	cout << "Red: " << someColor.Red << endl;
	cout << "Green: " << someColor.Green << endl;
	cout << "Blue: " << someColor.Blue << endl;
	cout << "Alpha: " << someColor.Alpha << endl;
	cout << "trying to create windows standad color table.." << endl;
	Input.CreateStandardColorTable();
	someColor = Input.GetColor(255);
	cout << "Red: " << someColor.Red << endl;
	cout << "Green: " << someColor.Green << endl;
	cout << "Blue: " << someColor.Blue << endl;
	cout << "Alpha: " << someColor.Alpha << endl;
	cout << "trying to create windows standad color table.." << endl;
	*/
}
