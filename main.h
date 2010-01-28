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
/*Camera Parameters */
float FOCAL_LENGTH = 1563.0; 
float THETA = 90; //degrees 

float X_WORLD = 0.0; //meters 
float Y_WORLD = 2.15; //meters
float Z_WORLD = 1.00; //meters
	
/* Functions */ 
int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void localMaximaSuppression(float image_row[], int row_size);
