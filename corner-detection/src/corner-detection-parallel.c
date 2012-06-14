#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include "libbmp/bmpfile.h"

#define DEPTH 8
#define MAX_INTENSITY 65025
#define HALF_WINDOW_WIDTH 1
#define HALF_NEIGHBORHOOD_WIDTH 15

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
		threshold = 20,
		n_thread = 8;

	float** cornerness_map = NULL;
	
	// arg count must be at least 2 to have a filename arg
	if ( argc < 3 )
	{
		printf("Usage: %s input_file output_file [threshold]\n", argv[0]);
		return 1;
	}

	if ( argc >= 4 )
		threshold = atoi(argv[3]);

	// Set number of threads.
	if ( argc >= 5 )	
		n_thread = atoi(argv[4]);
	omp_set_num_threads(n_thread);

	// read in the BMP
	bmp_read = bmp_create_8bpp_from_file(argv[1]);

	// if bmp_read is null, the file wasn't found
	if ( !bmp_read )
	{
		printf("File %s could not be found\n", argv[1]);
		return 1;
	}

	// get the height and width so we can build our map and new image
	width = bmp_get_dib(bmp_read).width;
    height = bmp_get_dib(bmp_read).height;

	// construct the cornerness map
	cornerness_map = (float**) malloc( sizeof(float*) * width );
	for ( x = 0; x < width; x++ )
		cornerness_map[x] = (float*) malloc( sizeof(float) * height );

	// calculate V_u,v(x,y)
	#pragma omp parallel for schedule(dynamic) shared(cornerness_map, bmp_read) private(x, y, u, v, a, b) firstprivate(threshold)
	for ( x = 0; x < width; x++ )	
	{
		for ( y = 0; y < height; y++ ) 
		{
			if ( x < 1 + HALF_WINDOW_WIDTH || x > width - 1 - (1 + HALF_WINDOW_WIDTH) || y < 1 + HALF_WINDOW_WIDTH || y > height - 1 - (1 + HALF_WINDOW_WIDTH) )
				cornerness_map[x][y] = 0;
			else
			{
				// the cornerness map should have the minumum V_u,v(x,y)
				float minV;
				float currV;
				minV = -1;
				for ( u = -1; u <= 1; u++ )	
				{
					for ( v = -1; v <= 1; v++ )	
					{
						if ( u != 0 || v != 0 )
						{
							currV = 0;
							for ( a = -HALF_WINDOW_WIDTH; a <= HALF_WINDOW_WIDTH; a++ )	
							{
								for ( b = -HALF_WINDOW_WIDTH; b <= HALF_WINDOW_WIDTH; b++ )	
								{
									rgb_pixel_t *pixel_A = bmp_get_pixel(bmp_read, x + u + a, y + v + b);
									float intensity_A = 0.2989 * (*pixel_A).red + 0.5870 * (*pixel_A).green + 0.1140 * (*pixel_A).blue;

									rgb_pixel_t *pixel_B = bmp_get_pixel(bmp_read, x + a, y + b);
									float intensity_B = 0.2989 * (*pixel_B).red + 0.5870 * (*pixel_B).green + 0.1140 * (*pixel_B).blue;

									currV += (intensity_A - intensity_B) * (intensity_A - intensity_B);
								}
							}
							if ( minV == -1 || currV < minV )
								minV = currV;
						}

					}
				}
				// only keep the values above the threshold
				if ( minV > threshold )
					cornerness_map[x][y] = minV;
				else 
					cornerness_map[x][y] = 0;
			}
		}
	}

	// image with corners highlighted
/*	bmp_with_corners = bmp_create(width, height, DEPTH);
	rgb_pixel_t highlight = {255,0,0,0};

	// modify original bmp to include highlight color
	printf("ncolors in bmp_read: %u\n", bmp_get_dib(bmp_read).ncolors);
	rgb_pixel_t* unusedColor;

	for ( x = 0; x < bmp_get_dib(bmp_read).ncolors; x++ )
		unusedColor = &(bmp_read->colors[x]);*/
	
	// draw picture without using nonmaximal supression
/*	for ( x = 0; x < width; x++ )
	{
		for ( y = 0; y < height; y++ )
		{
			if ( cornerness_map[x][y] > 0 )
			{
				bmp_set_pixel(bmp_with_corners, x, y, highlight);
			}
			else
			{
				rgb_pixel_t *pixel = bmp_get_pixel(bmp_read, x, y);
				bmp_set_pixel(bmp_with_corners, x, y, *pixel);
			}
		}
	}
*/
	 

	// draw picture with nonmaximal suppression
	rgb_pixel_t highlight = {255,0,0,0};
	#pragma omp parallel for schedule(dynamic) shared(cornerness_map, bmp_read, highlight) private(x, y, u, v)
	for ( x = HALF_NEIGHBORHOOD_WIDTH; x < width - HALF_NEIGHBORHOOD_WIDTH; x++)
	{
		for ( y = HALF_NEIGHBORHOOD_WIDTH; y < height - HALF_NEIGHBORHOOD_WIDTH; y++ )
		{
			int value = cornerness_map[x][y];
			int isMax = 1;
			for ( u = x-HALF_NEIGHBORHOOD_WIDTH; u <= x+HALF_NEIGHBORHOOD_WIDTH; u++ )
			{
				for ( v = y-HALF_NEIGHBORHOOD_WIDTH; v <= y+HALF_NEIGHBORHOOD_WIDTH; v++ )
				{
					if ( u != x || v != y )
					{
						if ( isMax == 1 && cornerness_map[u][v] < value )
							isMax = 1;
						else
							isMax = 0;
					}
				}
			}
			if (isMax == 1)
				bmp_set_pixel(bmp_read, x, y, highlight);
/*			else
				bmp_set_pixel(bmp_with_corners, x, y, *bmp_get_pixel(bmp_read, x, y));*/
		}
	}

	bmp_save(bmp_read, argv[2]);

	// clean up
	bmp_destroy(bmp_read);
	bmp_destroy(bmp_with_corners);
	for ( x = 0; x < width; x++ )
		free(cornerness_map[x]);
	free(cornerness_map);
}
