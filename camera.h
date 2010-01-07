#include <cv.h>
#include <stdio.h>
#include <math.h>

CvMat getK(CvMat* K);
CvMat getT(CvMat* T);
CvMat getR(CvMat* R);
CvMat createP(CvMat* K, CvMat* R, CvMat* T);
float degreesToRadians(float degrees);