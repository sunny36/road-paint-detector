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

const int ROW_DEBUG = 374;
/* Functions */ 
int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void localMaximaSuppression(const std::vector<float> image_row,
                            std::vector<float>& local_maxima,
                            int row_size);
void normalization(const std::vector<float> out, 
                   std::vector<float>& normalized,
                   int n, 
                   int lane_width);
void plotGraph(const char* filename);

#endif
