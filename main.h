#include <stdio.h>
#include <highgui.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "camera.h"
#include "convolution.h"

int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void localMaximaSuppression(float image_row[], int row_size);