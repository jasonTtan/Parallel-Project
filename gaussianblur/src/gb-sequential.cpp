#include"EasyBMP.h"
#include<cmath>
#include<stdlib.h>

const double PI = 3.14159;

using namespace std;

float Gaussian1D(int x, int sigma) {
	
	return exp(-x * x / (2 * sigma * sigma)) / sigma / sqrt(2 * PI);	
}

int GaussianBlur(char *input_filename, char *output_filename, int sigma = 1) {
		
	BMP input, gx, gy;
	int i, j, k;
	
	if (!input.ReadFromFile(input_filename))
		return -1;
	
	// Initialize intermediate outputs
	gx.SetSize(input.TellWidth(), input.TellHeight());
	gx.SetBitDepth(input.TellBitDepth());
	
	gy.SetSize(input.TellWidth(), input.TellHeight());
	gy.SetBitDepth(input.TellBitDepth());
			
	int filter_size = 2 * 3 * sigma + 1;
	float filter_sum = 0;
	
	float *gaussian_filter = new float[filter_size];
	
	// Initialize filter window using Gaussian distribution
	for (i = 0; i < filter_size; i++) {
		gaussian_filter[i] = Gaussian1D(abs(i - filter_size / 2), sigma);
		filter_sum += gaussian_filter[i];
	}
	for (i = 0; i < filter_size; i++) {
		gaussian_filter[i] /= filter_sum;
	}
	
	float red, green, blue;
	int pixel;
	ebmpBYTE pixelBYTE;
	
	// Apply 1D Gaussian filter horizontally
	for (j = 0; j < input.TellHeight(); j++)
		for (i = 0; i < input.TellWidth(); i++) {
			
			red = green = blue = 0;
			
			for (k = -filter_size / 2; k <= filter_size / 2; k++)
				if ((i + k >= 0) && (i + k < input.TellWidth())) {
					
					red += gaussian_filter[k + filter_size / 2] * input(i + k, j)->Red;
					green += gaussian_filter[k + filter_size / 2] * input(i + k, j)->Green;
					blue += gaussian_filter[k + filter_size / 2] * input(i + k, j)->Blue;
				}				
			
			/*gx(i, j)->Red = (ebmpBYTE) (red / filter_sum);
			gx(i, j)->Green = (ebmpBYTE) (green / filter_sum);
			gx(i, j)->Blue = (ebmpBYTE) (blue / filter_sum);*/
			gx(i, j)->Red = (ebmpBYTE) red;
			gx(i, j)->Green = (ebmpBYTE) green;
			gx(i, j)->Blue = (ebmpBYTE) blue;
		}
	
	// Apply 1D Gaussian filter vertically
	for (i = 0; i < gx.TellWidth(); i++)
		for (j = 0; j < gx.TellHeight(); j++) {
		
			red = green = blue = 0;
			
			for (k = -filter_size / 2; k <= filter_size / 2; k++)
				if ((j + k >= 0) && (j + k < gx.TellHeight())) {
					
					red += gaussian_filter[k + filter_size / 2] * gx(i, j + k)->Red;
					green += gaussian_filter[k + filter_size / 2] * gx(i, j + k)->Green;
					blue += gaussian_filter[k + filter_size / 2] * gx(i, j + k)->Blue;
				}
						
			/*gy(i, j)->Red = (ebmpBYTE) (red / filter_sum);
			gy(i, j)->Green = (ebmpBYTE) (green / filter_sum);
			gy(i, j)->Blue = (ebmpBYTE) (blue / filter_sum);*/
			gy(i, j)->Red = (ebmpBYTE) red;
			gy(i, j)->Green = (ebmpBYTE) green;
			gy(i, j)->Blue = (ebmpBYTE) blue;
		}
	
	// Create a grayscale color table if necessary
	if( gy.TellBitDepth() < 16 ) CreateGrayscaleColorTable(gy);
	// write the output file
	gy.WriteToFile(output_filename);
	
	delete[] gaussian_filter;
	return 0;
}

int main( int argc, char* argv[] )
{	
	GaussianBlur(argv[1], argv[2], atoi(argv[3]));	
	return 0;
}

