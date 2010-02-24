#include "camera.h"

Camera::Camera(float focal_length, int width, int height){
  const float THETA = 90; //degrees 
  const float X_CW = 0.0; //meters 
  const float Y_CW = 2.15; //meters
  const float Z_CW = 1.00; //meters

  K = cvCreateMat(3, 3, CV_32FC1);
  Rcw = cvCreateMat(3, 3, CV_32FC1);
  Tcw = cvCreateMat(3, 1, CV_32FC1);
  P = cvCreateMat(3, 4, CV_32FC1);

  //construct Rcw and Tcw
  this->setRcw(THETA);
  this->setTcw(X_CW, Y_CW, Z_CW);
  this->setK(focal_length, width, height);
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

void Camera::setRcw(float theta){
/*
Rx = 1        0               0
     0        cos(theta)      sin(theta)
     0        -sin(theta)    cos(theta)
*/	
	
	theta = degreesToRadians(theta); // theta in radians
	cvmSet(this->Rcw, 0, 0, 1);
	cvmSet(this->Rcw, 0, 1, 0);
	cvmSet(this->Rcw, 0, 2, 0);
	cvmSet(this->Rcw, 1, 0, 0);
	cvmSet(this->Rcw, 1, 1, 0);
	cvmSet(this->Rcw, 1, 2, 1);
	cvmSet(this->Rcw, 2, 0, 0);
	cvmSet(this->Rcw, 2, 1, -1);
	cvmSet(this->Rcw, 2, 2, 0);
	
  return;
}

CvMat* Camera::getRcw(){
  return Rcw;
}

void Camera::setTcw(float x, float y, float z){
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

  cvmSet(this->Tcw,0,0, x);
	cvmSet(this->Tcw,1,0, y);
	cvmSet(this->Tcw,2,0, z);	
  
  return;
}

CvMat* Camera::getTcw(){	
  return Tcw;
}

CvMat* Camera::getP(){
  CvMat *K, *R, *T; 
  K = this->getK();
	CvMat* RTcw = this->getRTcw();

  //IO io;
  /*io.printMatrixFloat(RTcw, RTcw->rows, RTcw->cols);*/

  CvMat* RTwc = cvCreateMat(4, 4, CV_32FC1); 
  cvInv(RTcw, RTwc); 

  CvMat RTwc_row_1_to_3;
  cvGetRows(RTwc, &RTwc_row_1_to_3, 0, 3); 
	cvMatMul(K, &RTwc_row_1_to_3, P);
  
  //io.printMatrixFloat(P, P->rows, P->cols); 

	return P;
}


CvMat* Camera::getRTcw(){
  Rcw = this->getRcw(); 
  Tcw = this->getTcw();
	CvMat* RTcw = cvCreateMat(4, 4, CV_32FC1);
	
	//copy Rcw to RTcw
  int row, col; 
  for(row = 0; row < Rcw->rows; row++){
    for(col = 0; col < Rcw->cols; col++){
      cvmSet(RTcw, row, col, cvmGet(Rcw, row, col));
    }
  }
  //copy Tcw to RTcw
  for(row = 0; row < Tcw->rows; row++){
    cvmSet(RTcw, row, 3, cvmGet(Tcw, row, 0));
  }
  //last row of RTcw = [0 0 0 1]
  cvmSet(RTcw, 3, 0, 0);
  cvmSet(RTcw, 3, 1, 0);
  cvmSet(RTcw, 3, 2, 0);
  cvmSet(RTcw, 3, 3, 1);
  
	return RTcw;
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


CvPoint2D32f Camera::imageToGroundPlane(CvPoint pt){
// 
//        | p_11 p_12 p_14 |
//   	H = | p_21 p_22 p_24 |
//        | p_31 p_32 p_34 |
// 
// 
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
 
  return cvPoint2D32f(cvmGet(ground_point, 0, 0), cvmGet(ground_point, 1, 0));
}
