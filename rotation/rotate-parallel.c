#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "bmpfile.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int min(int a, int b)
{
	if(a < b)
		return a;
	else return b;
}

int max(int a, int b)
{
	if(a > b)
		return a;
	else return b;
}

bmpfile_t* rotate(bmpfile_t *img, double rotationDegrees, int nthreads)
{
	bmpfile_t *rimg; // rotated image
	int width, height, rwidth, rheight;
	width = bmp_get_dib(img).width;
	height = bmp_get_dib(img).height;

	/* Calculate rotation in radians and then sine/cosine values
	   We do this here so we won't have to recalculate the values of cos and sin each time */
	double radians = rotationDegrees * M_PI / 180;
	double cosine = cos(radians);
	double sine = sin(radians);

	// Calculate rotated positions of image corners using middle as axis of rotation
	double wcos = width * cosine / 2;
	double wsin = width * sine / 2;
	double hcos = height * cosine / 2;
	double hsin = height * sine / 2;

	int x0 = (int)(-wcos - hsin);
	int y0 = (int)(-wsin + hcos);
	int x1 = (int)(wcos - hsin);
	int y1 = (int)(wsin + hcos);
	int x2 = (int)(wcos + hsin);
	int y2 = (int)(wsin - hcos);
	int x3 = (int)(-wcos + hsin);
	int y3 = (int)(-wsin - hcos);

	// Calculate dimensions of rotated image from image corners
	int xmin = min(x0, min(x1, min(x2, x3)));
	int xmax = max(x0, max(x1, max(x2, x3)));
	int ymin = min(y0, min(y1, min(y2, y3)));
	int ymax = max(y0, max(y1, max(y2, y3)));

	rwidth = xmax - xmin;
	rheight = ymax - ymin;

	// Construct rotated image pixel by pixel
	rimg = bmp_create(rwidth, rheight, 8);
	int xoffset, yoffset, rxoffset, ryoffset, srcx, srcy, destx, desty;
	rgb_pixel_t* srcpx;
	rgb_pixel_t fill = {200, 200, 200, 1};
	xoffset = width / 2;
	yoffset = height / 2;
	rxoffset = rwidth / 2;
	ryoffset = rheight / 2;

	omp_set_num_threads(nthreads);

	#pragma omp parallel for private(desty, srcx, srcy, srcpx) schedule(dynamic, 4)
	for(destx = 0; destx < rwidth; destx++)
		for(desty = 0; desty < rheight; desty++)
		{
			// Initialize pixels to gray
			bmp_set_pixel(rimg, destx, desty, fill); 
			
			/* For each rotated pixel, calculate source pixel using inverted rotation matrix
			   To rotate around center axis, first translate image to center, then rotate, then translate back */
			destx -= rxoffset; desty -= ryoffset;
			srcx = (destx*cosine) + (desty*sine) + xoffset;
			srcy = -(destx*sine) + (desty*cosine) + yoffset;
			destx += rxoffset; desty += ryoffset;

			// If source pixel is within original image, then set rotated pixel equal to source pixel
			if(srcx >= 0 && srcx < width && srcy >= 0 && srcy < height)
			{
				srcpx = bmp_get_pixel(img, srcx, srcy);
				bmp_set_pixel(rimg, destx, desty, *srcpx);
			}
		}
	return rimg;

}

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("Usage: ./rotate-seq <image file> <rotation degrees> <nthreads>\n");
		return 1;
	}
	
	// Read file arguments
	char* imgfile = argv[1];
	double rotationDegrees = atof(argv[2]);
	int nthreads = atoi(argv[3]);
	
	// Create BMP for input image
	bmpfile_t *img;
	if((img = bmp_create_8bpp_from_file(imgfile)) == NULL)
	{
		printf("Invalid bmp file: %s\n", imgfile);
		return 1;
	}

	// Rotate image and output rotated image
	time_t t1, t2;
	time(&t1);
	bmpfile_t *rimg = rotate(img, rotationDegrees, nthreads);
	time(&t2);
	printf("Time to perform parallel rotation using %d threads: %f seconds\n", nthreads, difftime(t2, t1));
	bmp_save(rimg, "test-out.bmp"); 

	return 0;
}
