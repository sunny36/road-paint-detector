#ifndef MAIN_H
#define MAIN_H

#include <cstdio>
#include <highgui.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "io.h"
#include "camera.h"
#include "convolution.h"
#include "contour.h"
#include "debug.h"

/* Constants */
const int ONE_PIXEL = 1; 
/*Camera Parameters */
float FOCAL_LENGTH = 1563.0; 

/* Functions */ 
int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void localMaximaSuppression(std::vector<float>& image_row, int row_size);
void normalization(std::vector<float>& out, int n, int lane_width);
void plotGraph(const char* filename);

#endif
