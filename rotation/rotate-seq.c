#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

bmpfile_t* rotate(bmpfile_t *img, double rotationDegrees)
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
	int xoffset, yoffset, rxoffset, ryoffset, destx, desty;
	double srcx, srcy;
	rgb_pixel_t fill = {200, 200, 200, 1};
	xoffset = width / 2;
	yoffset = height / 2;
	rxoffset = rwidth / 2;
	ryoffset = rheight / 2;

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

			/* If source pixel is within original image, then interpolate using bilinear interpolation
 			   to smooth out the rotation and set dest pixel to this interpolated pixel */
			if(srcx >= 0 && srcx <= width-1 && srcy >= 0 && srcy <= height-1)
			{
				// Calculate positions of nearest neighbor pixels
				int leftx = (int) srcx; int rightx = leftx + 1;
				int upy = (int) srcy; int downy = srcy + 1;

				// Values used for bilinear interpolation
				double dlx = (double) rightx - srcx;
				double drx = srcx - (double) leftx;
				double duy = (double) downy - srcy;
				double ddy = srcy - (double) upy;

				// Retrieve RGBA values of these pixels
				double ared, agreen, ablue, bred, bgreen, bblue, cred, cgreen, cblue, dred, dgreen, dblue;
				if(leftx >= 0 && upy >= 0)
				{
					rgb_pixel_t *lusrcpx = bmp_get_pixel(img, leftx, upy);
					ared = lusrcpx->red;
					agreen = lusrcpx->green;
					ablue = lusrcpx->blue;
				}

				if(rightx < width && upy >= 0)
				{
					rgb_pixel_t *rusrcpx = bmp_get_pixel(img, rightx, upy);
					bred = rusrcpx->red;
					bgreen = rusrcpx->green;
					bblue = rusrcpx->blue;
				}

				if(leftx >= 0 && downy < height)
				{
					rgb_pixel_t *ldsrcpx = bmp_get_pixel(img, leftx, downy);
					cred = ldsrcpx->red;
					cgreen = ldsrcpx->green;
					cblue = ldsrcpx->blue;
				}

				if(rightx < width && downy < height)
				{
					rgb_pixel_t *rdsrcpx = bmp_get_pixel(img, rightx, downy);
					dred = rdsrcpx->red;
					dgreen = rdsrcpx->green;
					dblue = rdsrcpx->blue;
				}
				
				// Calculate interpolated pixel values
				double red = ared*dlx*duy + bred*drx*duy + cred*dlx*ddy + dred*drx*ddy;
				double green = agreen*dlx*duy + bgreen*drx*duy + cgreen*dlx*ddy + dgreen*drx*ddy;
				double blue = ablue*dlx*duy + bblue*drx*duy + cblue*dlx*ddy + dblue*drx*ddy;
	
				// Set destination pixel to interpolated source pixel
				rgb_pixel_t srcpx;
				srcpx.red = (uint8_t) red;
				srcpx.green = (uint8_t) green;
				srcpx.blue = (uint8_t) blue;
				srcpx.alpha = 1;
				bmp_set_pixel(rimg, destx, desty, srcpx);
			}
		}
	return rimg;
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Usage: ./rotate-seq <image file> <rotation degrees>\n");
		return 1;
	}
	
	// Read file arguments
	char* imgfile = argv[1];
	double rotationDegrees = atof(argv[2]);
	
	// Create BMP for input image
	bmpfile_t *img;
	if((img = bmp_create_8bpp_from_file(imgfile)) == NULL)
	{
		printf("Invalid bmp file: %s\n", imgfile);
		return 1;
	}

	// Rotate image and save/output rotated image
	time_t t1, t2;
	time(&t1);
	bmpfile_t *rimg = rotate(img, rotationDegrees);
	time(&t2);
	printf("Time to perform sequential rotation: %d seconds\n", (int)difftime(t2, t1));
	bmp_save(rimg, "test-out.bmp"); 
	
	return 0;
}
