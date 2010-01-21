#include "convolution.h"

int Convolution::convolve1D(int* in, float* out, int dataSize, 
                            float* kernel, int kernelSize){
  int i, j, k;
  // check validity of params
  if(!in || !out || !kernel) return -1;
  if(dataSize <=0 || kernelSize <= 0) return -1;
  //start convolution from out[kernelSize-1] to out[dataSize-1] (last)
  //copyBorder(in, kernelSize);
  for(i = kernelSize-1; i < dataSize; ++i){
    out[i] = 0;                             // init to 0 before accumulate
    for(j = i, k = 0; k < kernelSize; --j, ++k){
      out[i] += in[j] * kernel[k];
    }	            
  }
  //    convolution from out[0] to out[kernelSize-2]
  for(i = 0; i < kernelSize - 1; ++i){
    out[i] = 0;                             // init to 0 before sum
    for(j = i, k = 0; j >= 0; --j, ++k){
      out[i] += in[j] * kernel[k];
    }            
  }

  for(i = 0; i < dataSize; i++){
    out[i] = out[i + kernelSize/2];
  }
  return 0;
}


float* Convolution::kernel1D(int w){
  float* kernel;
  int left_of_plus_one, right_of_plus_one, left_edge, right_edge;
  int j,k;
  //check if lane width is odd or even 
  if(w%2==0 && w!=0){
    //width is even
    kernel = (float *)calloc(sizeof(float), w*2+1); //kernel[0] -> kernel[w*2]
    kernel[w] = 1;
    left_of_plus_one = right_of_plus_one = ((w/2)-1);
    k = w; 
    k = k + 1;
    for(j=0; j< right_of_plus_one; j++){
      kernel[k] = 1;
      k = k +1;
    }
    k = w;
    k = k -1;
    for(j=0; j< left_of_plus_one; j++){
      kernel[k] = 1;
      k = k -1;
    }
    int left_edge = ((w/2)-1); 
    int right_edge = ((w/2)-1); 
    for(j=0; j< left_edge; j++){
      kernel[j] = -1;
    }
    kernel[j] = -0.5; j++; kernel[j]=0; 
    k = w*2+1;
    k = k-1;
    for(j=0; j < right_edge; j++){
      kernel[k] = -1; 
      k = k -1;
    }
    kernel[k] = -0.5; k--; kernel[k] = 0;
  }
  else{
    //width is odd
    kernel = (float *)calloc(sizeof(float), w*2+1);
    kernel[w] = 1;
    left_of_plus_one = right_of_plus_one = floor(w/2);
    k = w; 
    k = k + 1;
    for(j=0; j< right_of_plus_one; j++){
      kernel[k] = 1;
      k = k +1;
    }
    k = w;
    k = k -1;
    for(j=0; j< right_of_plus_one; j++){
      kernel[k] = 1;
      k = k -1;
    }
    left_edge = right_edge = floor(w/2);
    for(j=0; j< left_edge; j++){
      kernel[j] = -1;
    }
    kernel[j]= -0.5; 
    k = w*2 +1;
    k = k -1;
    for(j=0; j < right_edge; j++){
      kernel[k] = -1; 
      k = k -1;
    }
    kernel[k] = -0.5;
  }
  return kernel;
}

void Convolution::copyBorder(int* in, int kernel_width){
  IplImage* row = cvCreateImage(cvSize(640, 1), IPL_DEPTH_8U, 1); 
  //in => row 
  int i, j; 
  for(j = 0; j < row->width; j++){
    ((uchar*)(row->imageData + i*row->widthStep))[j] = in[j];
  }

  IplImage* row_b = cvCreateImage(cvSize(640 + kernel_width, 1), IPL_DEPTH_8U, 1); 
  CvPoint offset = cvPoint(kernel_width/2, 0); 
  cvCopyMakeBorder(row, row_b, offset, IPL_BORDER_REPLICATE, cvScalarAll(0));

  i = 0; 
  in = (int*)realloc(in, sizeof(int) * (640 + kernel_width));
  for(j = 0; j < row_b->width; j++){
    in[j] = ((uchar*)(row->imageData + i*row->widthStep))[j];
  }  
  return; 
}
