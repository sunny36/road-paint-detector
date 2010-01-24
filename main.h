#include <cstdio>
#include <highgui.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "io.h"
#include "camera.h"
#include "convolution.h"

int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void localMaximaSuppression(float image_row[], int row_size);