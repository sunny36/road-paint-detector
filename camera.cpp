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

float Camera::calculateY(int current_row){
	float Y=0, P_34=0, P_24=0, P_32=0, P_22=0;
	/*
	 Y = (v*P34 - P24) / (v*P32 - P22)
	 */
	P_34 = cvmGet(this->P, 2,3);
	P_24 = cvmGet(this->P, 1,3);
	P_32 = cvmGet(this->P, 2,1);
	P_22 = cvmGet(this->P, 1,1);
	Y = (current_row*P_34 - P_24) / (P_22 - current_row*P_32);		
	return Y;
}


CvPoint Camera::imageToGroundPlane(CvPoint pt){
/*
       | p_11 p_12 p_14 |
  H =  | p_21 p_22 p_24 |
       | p_31 p_32 p_34 |
       
*/
  CvMat *H = cvCreateMat( 3, 3, CV_32FC1); 
  //copy col 1 and 2 from K to H
  int row, col; 
  CvMat* P = getP();
  for(row = 0; row < P->rows ; row++){
    for(col = 0; col < 2; col++){
      cvmSet(H, row, col, cvmGet(P, row, col));
    }
  }
  //copy col 4 from K to H
  for(row = 0; row < P->rows; row++){
    cvmSet(H, row, 2, cvmGet(P, row, 3));
  }
#if defined(DEBUG)
  IO io;
  io.printMatrixFloat(P, P->rows, P->cols);
  printf("\n");
  io.printMatrixFloat(H, H->rows, H->cols);
#endif

  CvMat *H_inv = cvCreateMat( 3, 3, CV_32FC1); 
  cvInv(H, H_inv); 

  CvMat *image_point = cvCreateMat(3, 1, CV_32FC1);
  cvmSet(image_point, 0, 0, pt.x);
  cvmSet(image_point, 1, 0, pt.y);
  cvmSet(image_point, 2, 0, 1);

  CvMat *ground_point = cvCreateMat(3, 1, CV_32FC1);
  cvMatMul(H_inv, image_point, ground_point);

  /*IO io;*/
  /*io.printMatrixFloat(P, P->rows, P->cols);*/
  /*printf("\n");*/
  /*io.printMatrixFloat(H, H->rows, H->cols);*/
  /*printf("\n");*/
  /*io.printMatrixFloat(ground_point, ground_point->rows, ground_point->cols);*/
  /*printf("\n");*/
 
  return cvPoint(cvmGet(ground_point, 0, 0), cvmGet(ground_point, 1, 0));
}
