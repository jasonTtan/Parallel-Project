#include <stdlib.h>
#include <stdio.h>
#include "lib/libbmp/bmpfile.h"

#define DEPTH 8
#define MAX_INTENSITY 65025

int main(int argc, char** argv)
{
	// declare and initialize variables
	bmpfile_t *bmp_read = NULL,
			  *bmp_with_corners = NULL,
			  *bmp_gray = NULL;
	int x = 0,
		y = 0,
		u = 0,
		v = 0,
		a = 0,
		b = 0,
		width = 0,
		height = 0,
		window_x = 1,
		window_y = 1,
		threshold = 20;

	int** cornerness_map = NULL;
	
	// arg count must be at least 2 to have a filename arg
	if ( argc < 3 )
	{
		printf("Usage: %s input_file output_file [threshold]\n", argv[0]);
		return 1;
	}

	if ( argc == 4 )
		threshold = atoi(argv[3]);

	// read in the BMP
	bmp_read = bmp_create_8bpp_from_file(argv[1]);

	// if bmp_read is null, the file wasn't found
	if ( !bmp_read )
	{
		printf("File %s could not be found\n", argv[1]);
		return 1;
	}

	width = bmp_get_dib(bmp_read).width;
    height = bmp_get_dib(bmp_read).height;

	// grayscale the image
	bmp_gray = bmp_create(width, height, DEPTH);
	for ( x = 0; x < width; x++ )
	{
		for ( y = 0; y < height; y++ )
		{
			rgb_pixel_t *pixel = bmp_get_pixel(bmp_read, x, y);
			float grayscaled_color = 0.2989 * (*pixel).red + 0.5870 * (*pixel).green + 0.1140 * (*pixel).blue;
			(*pixel).red = (*pixel).green = (*pixel).blue = grayscaled_color;

			bmp_set_pixel(bmp_gray, x, y, *pixel);
		}
	}
	bmp_save(bmp_gray, "grayed.bmp");

	// construct the cornerness map
	cornerness_map = (int**) malloc( sizeof(int*) * width );
	for ( x = 0; x < width; x++ )
		cornerness_map[x] = (int*) malloc( sizeof(int) * height );

	// calculate V_u,v(x,y)
	for ( x = 0; x < width; x++ )	
	{
		for ( y = 0; y < height; y++ ) 
		{
			if ( x < 2 || x > width - 3 || y < 2 || y > height - 3 )
				cornerness_map[x][y] = 0;
			else
			{
				// the cornerness map should have the minumum V_u,v(x,y)
				int minV;
				int currV;
				for ( u = -1; u <= 1; u++ )	
				{
					for ( v = -1; v <= 1; v++ )	
					{
						minV = 0;
						currV = 0;
						for ( a = -1 ; a <= 1; a++ )	
						{
							for ( b = -1; b <= 1; b++ )	
							{
								rgb_pixel_t *intensity_A = bmp_get_pixel(bmp_gray, x + u + a, y + v + b);
								rgb_pixel_t *intensity_B = bmp_get_pixel(bmp_gray, x + a, y + b);
								if ( a == -1 && b == -1 )
									minV += ((*intensity_A).red - (*intensity_B).red) * ((*intensity_A).red - (*intensity_B).red);
								else
									currV += ((*intensity_A).red - (*intensity_B).red) * ((*intensity_A).red - (*intensity_B).red);		// arbitrarily choosing red but green and blue should
																																		// have the same intensity
							}
						}
						if ( currV < minV && currV > 0 )
							minV = currV;
					}
				}
				// only keep the values above the threshold
				if ( minV > threshold )
				{
					cornerness_map[x][y] = minV;
				}
			}
		}
	}

	// image with corners highlighted
	bmp_with_corners = bmp_create(width, height, DEPTH);
	rgb_pixel_t highlight = {0,255,255,0};
	int num_corners = 0;
	for ( x = 0; x < width; x++ )
	{
		for ( y = 0; y < height; y++ )
		{
			if ( cornerness_map[x][y] > 0 )
			{
				bmp_set_pixel(bmp_with_corners, x, y, highlight);
				num_corners++;
			}
			else
			{
				rgb_pixel_t *pixel = bmp_get_pixel(bmp_gray, x, y);
				bmp_set_pixel(bmp_with_corners, x, y, *pixel);
			}
		}
	}
	bmp_save(bmp_with_corners, argv[2]);
	printf("num_corners: %d\n", num_corners);
}
