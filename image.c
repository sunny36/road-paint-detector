#include <stdio.h>
#include <highgui.h>
#include "io.h"
#include "camera.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int calculateWidthInPixels(CvMat* P, float Y);
int convolve1D(int* in, int* out, int dataSize, int* kernel, int kernelSize);

typedef unsigned long long timestamp_t;

    static timestamp_t
    get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }

int main(int argc, char** argv){
  cvNamedWindow("MIT Road Paint Detector", CV_WINDOW_AUTOSIZE);
	//   CvCapture* capture = cvCreateFileCapture(argv[1]);
	// IplImage* frame;
	// IplImage* img = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	IplImage* img = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	CvMat* P = cvCreateMat(3, 4, CV_32FC1); 
	CvMat* K = cvCreateMat(3, 3, CV_32FC1);
	CvMat* R = cvCreateMat(3, 3, CV_32FC1);
	CvMat* T = cvCreateMat(3, 1, CV_32FC1);
	*K = getK(K);
	
	*T = getT(T);
	*R = getR(R);
	*P = createP(K, R, T);
	float Y=0, P_34=0, P_24=0, P_32=0, P_22=0;
	int row,j;
	int w;
	int *kernel;
	int shift_by;
	int out[640];
	int in[640];
	CvScalar s;
	timestamp_t t0;
	timestamp_t t1;
	timestamp_t total=0;
	// while(1){
	// 	frame = cvQueryFrame(capture);
	// 	cvCvtColor(frame,img,CV_RGB2GRAY);
	for(row = 0; row < img->height; row++){
		/*
			Y = (v*P34 - P24) / (v*P32 - P22)
		*/
		P_34 = cvmGet(P, 2,3);
		P_24 = cvmGet(P, 1,3);
		P_32 = cvmGet(P, 2,1);
		P_22 = cvmGet(P, 1,1);
		Y = (row*P_34 - P_24) / (row*P_32 - P_22);
		w = calculateWidthInPixels(P, Y);
		//printf("%d %d\n",row, w);
		if(w<0){
			w = 0; 
		}

		kernel = (int *)calloc(sizeof(int),w*2+2);
		
		for(j=0; j< w*2+2; j++){
			kernel[j] = -1;
		}
		kernel[0] = 0;
		kernel[(w*2+2) -1] = 0;
		shift_by = ceil(w/2);
		for(j=0; j<w; j++){
			kernel[j+shift_by] = 1;
		}

		for(j=0; j<640; j++){
			s = cvGet2D(img, row, j);
			in[j] = s.val[0];
		}
		timestamp_t t0 = get_timestamp();
		
		convolve1D(in,out,640,kernel, w*2+2);
		timestamp_t t1 = get_timestamp();
		total += (t1 - t0);
		
		for(j=0; j< 640; j++){
			if(out[j]<0){ 
				out[j] = 0;
			}
			s.val[0] = out[j];
			cvSet2D(img, row, j, s);
			
		}
		
	}
// 	cvShowImage("MIT Road Paint Detector", img);     
// 	char c = cvWaitKey(33);
// 	if(c==27) break;
// }
	fprintf(stderr, "%Lf", (total) / 1000000.0L);
	cvShowImage("MIT Road Paint Detector", img);     
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("MIT Read Paint Detector");
  // cvReleaseCapture(&capture); 
  // cvDestroyWindow("MIT Read Paint Detector");
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

	// // printf("\n");
	// // printMatrixFloat(1,4,P_3);
	// 
	// printf("\n");
	// printMatrixFloat(1,1,P_3_times_X_1);
	
	cvMatMul(P_1,X_2,P_1_times_X_2);
	cvMatMul(P_3,X_2,P_3_times_X_2);
	
	
	
	w = -((cvmGet(P_1_times_X_1,0,0) /
			 cvmGet(P_3_times_X_1,0,0)
			) 
			-
			(cvmGet(P_1_times_X_2,0,0) /
			 cvmGet(P_3_times_X_2,0,0)
			)); //minus sign is for experiment
	
	// printf("\n");
	// 	printMatrixFloat(1,4,P_1);
	// 	// 
	// cvmSet(P_1, 0,0, 0);
	// printf("\n");
	// printMatrixFloat(1,4,P_1);
	// 
	// printf("\n");
	// printMatrixFloat(3,4,P);
	
	return floor(w);
}

int convolve1D(int* in, int* out, int dataSize, int* kernel, int kernelSize)
{
    int i, j, k;

    // check validity of params
    if(!in || !out || !kernel) return -1;
    if(dataSize <=0 || kernelSize <= 0) return -1;

    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
    for(i = kernelSize-1; i < dataSize; ++i)
    {
        out[i] = 0;                             // init to 0 before accumulate

        for(j = i, k = 0; k < kernelSize; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    // convolution from out[0] to out[kernelSize-2]
    for(i = 0; i < kernelSize - 1; ++i)
    {
        out[i] = 0;                             // init to 0 before sum

        for(j = i, k = 0; j >= 0; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    return 0;
}
