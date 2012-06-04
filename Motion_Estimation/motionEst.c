#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmpfile.c"

#define blockSize 4

// creates a bmpfile, the image's width and height, and an array holding all of
// its color indexes.
int processBMP(char* imgName, bmpfile_t** b, int* height, int* width, int***bmpArray) {
  int i, x, y;
  
  // open the bmp files
  *b = bmp_create_8bpp_from_file(imgName);
  *width = bmp_get_width(*b);
  *height = bmp_get_height(*b);

  // create an array to store the bmp's color indexies
  *bmpArray = (int**) malloc(*height*sizeof(int*));
  if (*bmpArray == NULL) return -1;
  for (i = 0; i < *height; i++) {
    (*bmpArray)[i] = (int*) malloc(*height*sizeof(int));
    if ((*bmpArray)[i] == NULL) return -1;
  }

  // read bmp's color indexes into array
  for (y = 0; y < *height; y++) {
    for (x = 0; x < *width; x++) {
      (*bmpArray)[y][x] = find_closest_color(*b, *bmp_get_pixel(*b, x, y));
    }
  }
  
  return 0;
}

int main(int argc, char ** argv) {
  int i, x, y;
  int **bmp1Array, **bmp2Array;
  unsigned int width1, height1, width2, height2;
  bmpfile_t *bmp1, *bmp2, *bmpOut;
  char *imgName1 = NULL, *imgName2 = NULL;
  
  // get command line arguments
  if (argc != 3) {
	printf("Incorrect number of arguments.\n");
	return -1;
  } else {
    imgName1 = argv[1];
    imgName2 = argv[2];
  }

  // create bmpfiles, widths heights, and color index arrays
  if (processBMP(imgName1, &bmp1, &height1, &width1, &bmp1Array) != 0) return -1;
  if (processBMP(imgName2, &bmp2, &height2, &width2, &bmp2Array) != 0) return -1;

  // do stuff

  
  // create an output file
  bmpOut = bmp_create(width1, height1, 8);
  if (bmpOut == NULL) return -1;

  // set array colors to bmp
  for (y = 0; y < height1; y++) {
    for (x = 0; x < width1; x++) {
      bmp_set_pixel(bmpOut, x, y, *get_8bpp_color(bmp1, bmp1Array[y][x]));
    }
  }

  bmp_save(bmpOut, "motionEstOut.bmp");

  return 0;
}
