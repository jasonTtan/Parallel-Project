#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
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
    (*bmpArray)[i] = (int*) malloc(*width*sizeof(int));
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

int** copyArray(int** originalArray, int width, int height) {
  int i, x, y;
  int** copyArray;
  copyArray = (int**) malloc(height*sizeof(int*));
  if (copyArray == NULL) return NULL;
  for (i = 0; i < height; i++) {
    copyArray[i] = (int*) malloc(width*sizeof(int));
    if (copyArray[i] == NULL) return NULL;
  }
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      copyArray[y][x] = originalArray[y][x];
    }
  }
  return copyArray;
}

int getColor(int** bmpArray, int width, int height, int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height)
    // if out of bounds, just return 0
    return 0;
  return bmpArray[y][x];
}

int findMSE(int** bmp1Array, int** bmp2Array, int width, int height, int x1,
	    int y1, int x2, int y2, int blockWidth) {
  int i, j, total, difference;
  total = 0;
  for (i = 0; i < blockWidth; i++) {
    for (j = 0; j < blockWidth; j++) {
      difference = getColor(bmp1Array, width, height, x1 + j, y1 + i) -
	getColor(bmp2Array, width, height, x2 + j, y2 + i);
      total += difference * difference;
    }
  }
  return total / (blockWidth * blockWidth);
}

void findNewPos(int** bmp1Array, int** bmp2Array, int width, int height, int x1,
		int y1, int* x2, int* y2, int blockWidth, int searchBoundXLow,
		int searchBoundXHigh, int searchBoundYLow, int searchBoundYHigh) {
  int i, j, lowestMSE, curMSE;
  lowestMSE = INT_MAX;

  for (i = searchBoundYLow; i < searchBoundYHigh; i++) {
    for (j = searchBoundXLow; j < searchBoundXHigh; j++) {
      curMSE = findMSE(bmp1Array, bmp2Array, width, height, x1, y1, j, i,
		       blockWidth);
      if (curMSE < lowestMSE) {
	lowestMSE = curMSE;
	*x2 = j;
	*y2 = i;
      }
    }
  }
}

void plotPoint(int** bmpArray, int width, int height, int x, int y, int color) {
  if (x > 0 && x < width && y > 0 && y < height)
    bmpArray[y][x] = color;
}

void drawVector(int** bmpArray, int width, int height, int x1, int y1, int x2, int y2) {
  float x,y,xinc,yinc,dx,dy;
  int k, step, i, j;
  dx=x2-x1;
  dy=y2-y1;
  if(abs(dx)>abs(dy))
    step=abs(dx);
  else
    step=abs(dy);
  xinc=dx/step;
  yinc=dy/step;
  x=x1;
  y=y1;
  plotPoint(bmpArray, width, height, (int ) x, (int) y, 0);
  for(k=1;k<=step;k++)
    {
      x=x+xinc;
      y=y+yinc;
      plotPoint(bmpArray, width, height, (int ) x, (int) y, 0);
    }

  // draw a square around the start point to make it easier to read
  for (i = y1 - 1; i < y1 + 1; i++) {
    for (j = x1 - 1; j < x1 + 1; j++) {
      plotPoint(bmpArray, width, height, j, i, 0);
    }
  }
}

void findSearchBounds(int width, int height, int searchPadding,
		     int topLeftX, int topLeftY, int blockWidth,
		     int* searchBoundXLow, int* searchBoundXHigh,
		     int* searchBoundYLow, int* searchBoundYHigh) {
  *searchBoundXLow = topLeftX - searchPadding;
  if (*searchBoundXLow < 0)
    *searchBoundXLow = 0;
  *searchBoundXHigh = topLeftX + blockWidth + searchPadding;
  if (*searchBoundXHigh > width)
    *searchBoundXHigh = width;
  *searchBoundYLow = topLeftY - searchPadding;
  if (*searchBoundYLow < 0)
    *searchBoundYLow = 0;
  *searchBoundYHigh = topLeftY + blockWidth + searchPadding;
  if (*searchBoundYHigh > height)
    *searchBoundYHigh = height;
}

void motionEstimation(int** bmp1Array, int** bmp2Array, int** outArray,
		      int width, int height, int blockWidth, int searchPadding) {
  int i, j, searchBoundXLow, searchBoundXHigh, searchBoundYLow, searchBoundYHigh;
  int newX, newY;
  
  for (i = 0; i < height; i += blockWidth) {
    for (j = 0; j < width; j += blockWidth) {
      // use findSearchBounds to get bounds to search
      findSearchBounds(width, height, searchPadding, j, i, blockWidth,
		       &searchBoundXLow, &searchBoundXHigh, &searchBoundYLow,
		       &searchBoundYHigh);
      // use findNewPos to get new pos
      findNewPos(bmp1Array, bmp2Array, width, height, j, i, &newX, &newY,
		 blockWidth, searchBoundXLow, searchBoundXHigh, searchBoundYLow,
		 searchBoundYHigh);
      // use drawVector to draw vector on the output array
      drawVector(outArray, width, height, j + (blockWidth/2), i + (blockWidth/2),
		 newX + (blockWidth/2), newY + (blockWidth/2));
    }
  }
}


int main(int argc, char ** argv) {
  int i, j, x, y, blockWidth, searchPadding;
  int **bmp1Array, **bmp2Array, **outArray;
  unsigned int width1, height1, width2, height2;
  bmpfile_t *bmp1, *bmp2, *bmpOut;
  char *imgName1 = NULL, *imgName2 = NULL;

  blockWidth = 16;
  searchPadding = 8;
  
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

  outArray = copyArray(bmp1Array, width1, height1);
  if (outArray == NULL) return -1;

  // do stuff
  motionEstimation(bmp1Array, bmp2Array, outArray, width1, height1, blockWidth,
		   searchPadding);

  // create an output file
  bmpOut = bmp1;
  if (bmpOut == NULL) return -1;

  // set array colors to bmp
  for (y = 0; y < height1; y++) {
    for (x = 0; x < width1; x++) {
      bmp_set_pixel(bmpOut, x, y, *get_8bpp_color(bmpOut, outArray[y][x]));
    }
  }

  bmp_save(bmpOut, "motionEstOut.bmp");

  return 0;
}
