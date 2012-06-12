#include <stdio.h>
#include <stdlib.h>
#include "bmpfile.c"

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

void rotate(bmpfile_t *img, float rotationDegrees)
{
	bmpfile_t *rimg; // rotated image
	int width, height, rwidth, rheight;
	width = bmp_get_dib(img).width;
	height = bmp_get_dib(img).height;

	// Calculate rotation in radians and then sine/cosine values
	float radians = rotationDegrees * M_PI / 180;
	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	// Calculate rotated positions of image corners using middle as axis of rotation
	float wcos = width * cosine / 2;
	float wsin = width * sine / 2;
	float hcos = height * cosine / 2;
	float hsin = height * sine / 2;

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
	int xoffset, yoffset, srcx, srcy, destx, desty;
	rgb_pixel_t fill = {200, 200, 200, 1};
	xoffset = rwidth / 2;
	yoffset = rheight / 2;

	/*for(destx = 0; destx < rwidth; destx++)
		for(desty = 0; desty < rheight; desty++)
			bmp_set_pixel(rimg, destx, desty, fill);
		

	for(srcx = 0; srcx < width; srcx++)
		for(srcy = 0; srcy < height; srcy++)
		{
			destx = ((srcx - rwidth/2)*cosine) - ((srcy - rheight/2)*sine) + rwidth/2;
			desty = ((srcx - rwidth/2)*sine) + ((srcy - rheight/2)*cosine) + rheight/2;
			
			if(destx >= 0 && destx < rwidth && desty >= 0 && desty < rheight)
			{
				rgb_pixel_t* srcpx = bmp_get_pixel(img, srcx, srcy);
				bmp_set_pixel(rimg, destx, desty, *srcpx);
			}
		}*/

	int chunksize = 4;

	#pragma omp parallel for schedule(dynamic, chunksize)
	for(destx = 0; destx < rwidth; destx++)
		for(desty = 0; desty < rheight; desty++)
		{
			// Initialize pixels to gray
			bmp_set_pixel(rimg, destx, desty, fill); 
			
			// For each rotated pixel, calculate source pixel using inverted rotation matrix
			destx -= xoffset; desty -= yoffset;
			srcx = (destx*cosine) + (desty*sine) + yoffset;
			srcy = -(destx*sine) + (desty*cosine) + xoffset;
			destx += xoffset; desty += yoffset;

			// If source pixel is within original image, then set rotated pixel equal to source pixel
			if(srcx >= 0 && srcx < width && srcy >= 0 && srcy < height)
			{
				rgb_pixel_t* srcpx = bmp_get_pixel(img, srcx, srcy);
				bmp_set_pixel(rimg, destx, desty, *srcpx);
			}
		}

	// Output file
	bmp_save(rimg, "test-out.bmp"); 

}

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("Usage: ./rotate-seq <image file> <rotation degrees> <nthreads>\n");
	}
	
	char* imgfile = argv[1];
	float rotationDegrees = atof(argv[2]);
	int nthreads = atoi(argv[3]);
	
	bmpfile_t *img;
	if((img = bmp_create_8bpp_from_file(imgfile)) == NULL)
	{
		printf("Invalid bmp file: %s\n", imgfile);
		return 1;
	}

	omp_set_num_threads(nthreads);

	rotate(img, rotationDegrees);

}
