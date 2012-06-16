#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include "lib/libbmp/bmpfile.h"

#define DEPTH 8
#define MAX_INTENSITY 65025
#define HALF_WINDOW_WIDTH 1
#define HALF_NEIGHBORHOOD_WIDTH 5
inline float  Max   ( int  x, int  y ) { return x > y ? x : y; }

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
		n_thread = 8,
		currV = 0,
		minV = -1;

	int** cornerness_map = NULL;
	
	// arg count must be at least 2 to have a filename arg
	if ( argc < 3 )
	{
		printf("Usage: %s input_file output_file [threshold] [n_thread]\n", argv[0]);
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
	cornerness_map = (int**) malloc( sizeof(int*) * width );
	for ( x = 0; x < width; x++ )
		cornerness_map[x] = (int*) malloc( sizeof(int) * height );

	// calculate V_u,v(x,y)
	#pragma omp parallel private(x, y, u, v, a, b) shared(minV, currV)
	{
		for ( x = 0; x < width; x++ )	
		{
			for ( y = 0; y < height; y++ ) 
			{
				if ( x < 1 + HALF_WINDOW_WIDTH || x > width - 1 - (1 + HALF_WINDOW_WIDTH) || y < 1 + HALF_WINDOW_WIDTH || y > height - 1 - (1 + HALF_WINDOW_WIDTH) )
					cornerness_map[x][y] = 0;
				else
				{
					// the cornerness map should have the minumum V_u,v(x,y)
					for ( u = -1; u <= 1; u++ )	
					{
						for ( v = -1; v <= 1; v++ )	
						{
							minV = -1;
							currV = 0;
							for ( a = -HALF_WINDOW_WIDTH; a <= HALF_WINDOW_WIDTH; a++ )	
							{
								#pragma omp for schedule(dynamic) reduction(+ : currV)
								for ( b = -HALF_WINDOW_WIDTH; b <= HALF_WINDOW_WIDTH; b++ )	
								{
									rgb_pixel_t *pixel_A = bmp_get_pixel(bmp_read, x + u + a, y + v + b);
									float intensity_A = 0.2989 * (*pixel_A).red + 0.5870 * (*pixel_A).green + 0.1140 * (*pixel_A).blue;

									rgb_pixel_t *pixel_B = bmp_get_pixel(bmp_read, x + a, y + b);
									float intensity_B = 0.2989 * (*pixel_B).red + 0.5870 * (*pixel_B).green + 0.1140 * (*pixel_B).blue;

									currV += (intensity_A - intensity_B) * (intensity_A - intensity_B);
								}
							}
							#pragma omp critcal
							{
								if ( minV == -1 || currV < minV && currV > 0 )
									minV = currV;
							}
						}
					}
					// only keep the values above the threshold
					if ( minV > threshold )
						cornerness_map[x][y] = minV;
				}
			}
		}
	}

	// perform non-maximal suppression
	/*
	int* corners = (int*) malloc( sizeof(int)*2*width*height );
	memset( corners, -1, sizeof(int)*2*width*height );
	int corner_count = 0;
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
					isMax = isMax && (cornerness_map[u][v]<=value);
				}
			}
			if (isMax)
			{
				printf("(%d, %d) is a corner\n", x, y);
				corners[corner_count*2] = x;
				corners[corner_count*2 + 1] = y;
				corner_count++;
			}
		}
	}
	*/

	// image with corners highlighted
	bmp_with_corners = bmp_create(width, height, DEPTH);
	rgb_pixel_t highlight = {0,255,255,0};
	
	#pragma omp parallel
	{
		for ( x = 0; x < width; x++ )
		{
			#pragma omp for
			for ( y = 0; y < height; y++ )
			{
				if ( cornerness_map[x][y] > 0 )
				{
					bmp_set_pixel(bmp_with_corners, x, y, highlight);
				}
				else
				{
					rgb_pixel_t *pixel = bmp_get_pixel(bmp_read, x, y);
					bmp_set_pixel(bmp_with_corners, x, y, *get_8bpp_color(bmp_with_corners, find_closest_color(bmp_with_corners, *pixel)));
				}
			}
		}
	}
	bmp_save(bmp_with_corners, argv[2]);
	
	/*
	for ( x = 0; x < corner_count; x++ )
	{
		//printf("(%d, %d) is a corner\n", corners[x*2], corners[x*2+1]);
		bmp_set_pixel(bmp_read, corners[x*2], corners[x*2+1], highlight);
	}
	bmp_save(bmp_read, argv[2]);
	printf("corner_count: %d\n", corner_count);
	*/

	// TODO: clean up
	bmp_destroy(bmp_read);
	bmp_destroy(bmp_with_corners);
	for ( x = 0; x < width; x++ )
		free(cornerness_map[x]);
	free(cornerness_map);
}
