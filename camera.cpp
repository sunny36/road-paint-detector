#include "camera.h"

Camera::Camera(){
  K = cvCreateMat(3, 3, CV_32FC1);
  R = cvCreateMat(3, 3, CV_32FC1);
  T = cvCreateMat(3, 1, CV_32FC1);
  P = cvCreateMat(3, 4, CV_32FC1);
}

void Camera::setK(float focal_length, int width, int height){
 	float p_x = width / 2; 
	float p_y = height / 2; 
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 0, 0)) = focal_length;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 0, 1)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 0, 2)) = p_x;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 1, 0)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 1, 1)) = focal_length;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 1, 2)) = p_y;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 2, 0)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 2, 1)) = 0;
	*((float*)CV_MAT_ELEM_PTR(*(this->K), 2, 2)) = 1;

  return;
}

CvMat* Camera::getK(){	
	return K;
}

void Camera::setR(float theta){
/*
Rx = 1        0               0
     0        cos(theta)      sin(theta)
     0        -sinc(theta)    cos(theta)
*/	
	
	theta = degreesToRadians(theta); // theta in radians
	cvmSet(this->R, 0, 0, 1);
	cvmSet(this->R, 0, 1, 0);
	cvmSet(this->R, 0, 2, 0);
	cvmSet(this->R, 1, 0, 0);
	cvmSet(this->R, 1, 1, cos(theta));
	cvmSet(this->R, 1, 2, sin(theta));
	cvmSet(this->R, 2, 0, 0);
	cvmSet(this->R, 2, 1, -sin(theta));
	cvmSet(this->R, 2, 2, cos(theta));
	
  return;
}

CvMat* Camera::getR(){
  return R;
}

void Camera::setT(float x, float y, float z){
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

  cvmSet(this->T,0,0, x);
	cvmSet(this->T,1,0, y);
	cvmSet(this->T,2,0, z);	
  
  return;
}

CvMat* Camera::getT(){	
  return T;
}

CvMat* Camera::getP(){
  CvMat *K, *R, *T; 
  K = this->getK();
  R = this->getR(); 
  T = this->getT();
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
	return P;
}

float Camera::degreesToRadians(float degrees){
	return (degrees * (3.14/180)); 
}
