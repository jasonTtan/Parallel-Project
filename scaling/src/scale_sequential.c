#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bmpfile.h"

void bmp_to_array(int* pixels, bmpfile_t* bmp_read, int width, int height ){

	int i, j;
	int offset = 0;
	
	for( i = 0; i < width; ++i){
		for( j= 0; j < height; ++j){
			rgb_pixel_t *pixel = bmp_get_pixel(bmp_read, i, j);
			pixels[offset] = find_closest_color( bmp_read, *pixel);	
			offset++;
		}
	}

}

bmpfile_t* bilinear_resize(bmpfile_t* bmp_read, float scale, int depth){

	bmpfile_t *bmp_resize = NULL;
	int width, height;
	int en_width, en_height;
	int a, b, c, d, x, y, index;
	int i, j;
    float x_diff, y_diff, blue, red, green ;
    int offset = 0 ;
	int color_holder;

	width = bmp_get_dib(bmp_read).width;
	height = bmp_get_dib(bmp_read).height; 
	
	en_width = (int)(width * scale);
	en_height = (int)(height * scale);
	
	float x_ratio = ((float)(width-1))/en_width ;
    float y_ratio = ((float)(height-1))/en_height ;
	
	int temp[en_width*en_height];
	int pixels[ width*height];
	
	bmp_to_array( pixels, bmp_read, width, height);
	
	bmp_resize = bmp_create(en_width, en_height, 8);
	
	// do bilinear interpolatin to get new picture
	if( bmp_resize != NULL){
    for (i = 0; i < en_width; ++i) {
      for (j = 0; j < en_height; ++j) {
	           	  
			x = (int)(x_ratio * i) ;
            y = (int)(y_ratio * j) ;
            x_diff = (x_ratio * i) - x ;
            y_diff = (y_ratio * j) - y ;
			index = (x*height + y);
         				
			rgb_pixel_t *a = get_8bpp_color(bmp_read, pixels[index]);
			rgb_pixel_t *b = get_8bpp_color(bmp_read, pixels[index+height]);
			rgb_pixel_t *c = get_8bpp_color(bmp_read, pixels[index+1]);		
			rgb_pixel_t *d = get_8bpp_color(bmp_read, pixels[index+height+1]);
			
			int ared = a->red;
			int agreen = a->green;
			int ablue = a->blue;
			int bred = b->red;
			int bgreen = b->green;
			int bblue = b->blue;
			int cred = c->red;
			int cgreen = c->green;
			int cblue = c->blue;
			int dred = d->red;
			int dgreen = d->green;
			int dblue = d->blue;
								
			red = ared*(1-x_diff)*(1-y_diff) + bred*(x_diff)*(1-y_diff) + cred*y_diff*(1-x_diff) + dred*(x_diff*y_diff);
			green = agreen*(1-x_diff)*(1-y_diff) + bgreen*(x_diff)*(1-y_diff) + cgreen*y_diff*(1-x_diff) + dgreen*(x_diff*y_diff);
			blue = ablue*(1-x_diff)*(1-y_diff) + bblue*(x_diff)*(1-y_diff) + cblue*y_diff*(1-x_diff) + dblue*(x_diff*y_diff);

			rgb_pixel_t *curr_pixel;
			curr_pixel->red = (uint8_t)red;
			curr_pixel->green = (uint8_t)green;
			curr_pixel->blue = (uint8_t)blue;
			curr_pixel->alpha = 0;
			bmp_set_pixel(bmp_resize, i, j, *curr_pixel);					
      }
    }
	}
	
	return bmp_resize;
}

bmpfile_t * bmp_scale(char* bmp_image_name, float scale, int depth ){

	bmpfile_t *bmp_read = NULL;
	bmpfile_t *bmp_resized = NULL;
	int i, j;
	int width, height;
	rgb_pixel_t pixel = {128, 64, 0, 0};
	
	//bmp now holds the read in bitmap image
	bmp_read = bmp_create_8bpp_from_file(bmp_image_name);
  
    if (bmp_read != NULL) {
		//if( scale >= 1)
			bmp_resized = bilinear_resize( bmp_read, scale, depth);	
	}
	
	else if( bmp_read == NULL)
		printf("bitmap could not be read!\n");
	
	bmp_destroy(bmp_read);
	return bmp_resized;
}

int main(int argc, char **argv) {
	
	bmpfile_t *bmp_resized = NULL;
	bmpfile_t *result = NULL;
	int width, height,depth, i, j;
	float scale;
	
	// Parse argv.
	scale = (float)atof(argv[2]);
	
	bmp_resized = bmp_scale( argv[1], scale, depth);
	width = bmp_get_dib(bmp_resized).width;
	height = bmp_get_dib(bmp_resized).height; 
	
	result = bmp_create(width, height, 8);	
	if( result != NULL){
	for (i = 0; i < width; ++i) {
      for (j = 0; j < height; ++j) {
		rgb_pixel_t *p = bmp_get_pixel(bmp_resized, i, j);
		bmp_set_pixel(result, i, j, *p);	
	  }
	}
	bmp_save(result, "bmp_scale.bmp");
	}
	
    bmp_destroy(bmp_resized);
    return 0;
	

}
