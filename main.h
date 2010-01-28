#include <cstdio>
#include <highgui.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "io.h"
#include "camera.h"
#include "convolution.h"

/* Constants */
const int ONE_PIXEL = 1; 
float FOCAL_LENGTH = 1563.0; 

/* Functions */ 
int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void localMaximaSuppression(float image_row[], int row_size);
