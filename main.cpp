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


int main(int argc, char** argv){
	cvNamedWindow("MIT Road Paint Detector", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	IplImage* frame;
	IplImage* img = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	
	CvMat* P = cvCreateMat(3, 4, CV_32FC1); 
	CvMat* K = cvCreateMat(3, 3, CV_32FC1);
	CvMat* R = cvCreateMat(3, 3, CV_32FC1);
	CvMat* T = cvCreateMat(3, 1, CV_32FC1);
	
	Camera camera;
	*K = camera.getK(K);
	
	*T = camera.getT(T);
	*R = camera.getR(R);
	*P = camera.setP(K, R, T);
	float Y;
	int row,j;
	int w;
	int *kernel;
	int shift_by;
	int out[640];
	int in[640];
	CvScalar s;
	int max=0;
	Convolution convolution;
	
	while(1){
		frame = cvQueryFrame(capture);
		cvCvtColor(frame,img,CV_RGB2GRAY);
		for(row = 0; row < img->height; row++){
			Y = calculateY(P, row);
			w = calculateWidthInPixels(P, Y);
			
			if(w<0){
				w = 0; 
			}
			
			kernel = (int *)calloc(sizeof(int),w*2);
			
			for(j=0; j< w*2; j++){
				kernel[j] = -1;
			}
			// kernel[0] = 0;
			// kernel[(w*2+2) -1] = 0;
			shift_by = ceil(w/2);
			for(j=0; j<w; j++){
				kernel[j+shift_by] = 1;
			}
			
			for(j=0; j<640; j++){
				s = cvGet2D(img, row, j);
				in[j] = s.val[0];
			}
			
			
			convolution.convolve1D(in,out,640,kernel, w*2);
			
			for(j=0; j< 640; j++){
				if(out[j]<0){ 
					out[j] = 0;
					s.val[0] = out[j];
				}
				else if(out[j]!=0){
					s.val[0] = out[j];
				}
				
				cvSet2D(img, row, j, s);			
			}
			
			for(j = 1; j < 640 - 1; j = j++){
				
				s = cvGet2D(img, row, j);
				if(s.val[0] < (cvGet2D(img, row, j-1)).val[0] && s.val[0] < (cvGet2D(img, row, j+1)).val[0] ){

					s.val[0] =0.0;
					cvSet2D(img, row, j, s);
				} 
				
			}
			
		}
		cvShowImage("MIT Road Paint Detector", img);     
		char c = cvWaitKey(33);
		if(c==27) break;
	}
	cvReleaseCapture(&capture); 
	cvDestroyWindow("MIT Read Paint Detector");
	return 0;
}

int calculateWidthInPixels(CvMat* P, float Y){
	float W = 0.2; //width of road 20cm ~ 0.2m 
	float w = 0.0; //width of the roads in pixels
	
	CvMat tmp;
	//create P_1 (row 1 of matrix P)
	CvMat *P_1 = cvCreateMat(1,4,CV_32FC1);
	cvGetRow(P,&tmp,0); //row 0
	cvCopy(&tmp,P_1,NULL);
	
	CvMat *P_3 = cvCreateMat(1,4,CV_32FC1);
	cvGetRow(P,&tmp,2); //row 2
	cvCopy(&tmp,P_3,NULL);
	
	CvMat* X_1 = cvCreateMat(4,1,CV_32FC1);
	CvMat* X_2 = cvCreateMat(4,1,CV_32FC1);
	CvMat* P_1_times_X_1 = cvCreateMat(1,1,CV_32FC1);
	CvMat* P_3_times_X_1 = cvCreateMat(1,1,CV_32FC1);
	CvMat* P_1_times_X_2 = cvCreateMat(1,1,CV_32FC1);
	CvMat* P_3_times_X_2 = cvCreateMat(1,1,CV_32FC1);
	
	cvmSet(X_1,0,0,W);
	cvmSet(X_1,1,0,Y);
	cvmSet(X_1,2,0,0.0);
	cvmSet(X_1,3,0,1.0);
	
	cvmSet(X_2,0,0,0);
	cvmSet(X_2,1,0,Y);
	cvmSet(X_2,2,0,0);
	cvmSet(X_2,3,0,1);
	
	cvMatMul(P_1,X_1,P_1_times_X_1);
	cvMatMul(P_3,X_1,P_3_times_X_1);
	
	
	cvMatMul(P_1,X_2,P_1_times_X_2);
	cvMatMul(P_3,X_2,P_3_times_X_2);
	
	
	
	w = -((cvmGet(P_1_times_X_1,0,0) /
		   cvmGet(P_3_times_X_1,0,0)
		   ) 
		  -
		  (cvmGet(P_1_times_X_2,0,0) /
		   cvmGet(P_3_times_X_2,0,0)
		   )); //minus sign is for experiment
	
	
	return floor(w);
}

float calculateY(CvMat* P, int current_row){
	float Y=0, P_34=0, P_24=0, P_32=0, P_22=0;
	/*
	 Y = (v*P34 - P24) / (v*P32 - P22)
	 */
	P_34 = cvmGet(P, 2,3);
	P_24 = cvmGet(P, 1,3);
	P_32 = cvmGet(P, 2,1);
	P_22 = cvmGet(P, 1,1);
	Y = (current_row*P_34 - P_24) / (current_row*P_32 - P_22);		
	return Y;
}

