#include "camera.h"

CvMat getK(CvMat* K){	
	float f = 1563.0; 
	float p_x = 640 / 2; 
	float p_y = 480 / 2; 
	*((float*)CV_MAT_ELEM_PTR(*K, 0, 0)) = f;
	*((float*)CV_MAT_ELEM_PTR(*K, 0, 1)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*K, 0, 2)) = p_x;
	*((float*)CV_MAT_ELEM_PTR(*K, 1, 0)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*K, 1, 1)) = f;
	*((float*)CV_MAT_ELEM_PTR(*K, 1, 2)) = p_y;
	*((float*)CV_MAT_ELEM_PTR(*K, 2, 0)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*K, 2, 1)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*K, 2, 2)) = 1;
	
	return *K;
}

CvMat getR(CvMat* R){
/*
Rx = 1        0               0
     0        cos(theta)      sin(theta)
     0        -sinc(theta)    cos(theta)
*/	
	float theta = 90; //degrees 
	theta = degreesToRadians(theta); // theta in radians
	cvmSet(R, 0, 0, 1);
	cvmSet(R, 0, 1, 0);
	cvmSet(R, 0, 2, 0);
	cvmSet(R, 1, 0, 0);
	cvmSet(R, 1, 1, cos(theta));
	cvmSet(R, 1, 2, sin(theta));
	cvmSet(R, 2, 0, 0);
	cvmSet(R, 2, 1, -sin(theta));
	cvmSet(R, 2, 2, cos(theta));
	
	return *R;
}

CvMat getT(CvMat* T){	
/*
World Co-ordinate system 
	Z
	 |
   |
	 |
	 |_ _ _ _ Y
	 \
	  \
	   \
	    \ X
*/

	float x = 0.0; 
	float y = 2.15; 
	float z = 1.00;
	cvmSet(T,0,0, x);
	cvmSet(T,1,0, y);
	cvmSet(T,2,0, z);	
	
	return *T;
}

CvMat createP(CvMat* K, CvMat* R, CvMat* T){
	CvMat* Rt = cvCreateMat(3, 4, CV_32FC1); //concatenates R and T
	CvMat* P = cvCreateMat(3, 4, CV_32FC1);
	int row, col;
	for(row = 0; row < R->rows; row++){
		for(col =0; col < R->cols; col++){
			cvmSet(Rt, row, col, cvmGet(R, row, col));
		}		
	}
	
	for(row = 0; row < T->rows; row++){
		for(col = 0; col < T->cols; col++){
			cvmSet(Rt, row, col + 3, cvmGet(T, row, col));
		}
	}
	
	cvMatMul(K, Rt, P);
	return *P;
}

float degreesToRadians(float degrees){
	return (degrees * (3.14/180)); 
}