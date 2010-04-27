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

const int ROW_DEBUG = 361;
/* Functions */ 
int calculateWidthInPixels(CvMat* P, float Y);
float calculateY(CvMat* P, int current_row);
void normalization(const std::vector<float> out, 
                   std::vector<float>& normalized,
                   int n, 
                   int lane_width);
#endif
