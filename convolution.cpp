#include "convolution.h"

int Convolution::convolve1D(int* in, float* out, int dataSize, 
                            float* kernel, int kernelSize){
  int i, j, k;
	float *temp_float; 
  // check validity of params
  if(!in || !out || !kernel) return -1;
  if(dataSize <=0 || kernelSize <= 0) return -1;
  
	int *in_with_borders = NULL;
	int new_row_width = (640 + (kernelSize - 1));
	in_with_borders = (int*)calloc(sizeof(int), new_row_width);
	//left edge
	for(i = 0; i < (kernelSize-1)/2; i++){
		in_with_borders[i] = in[0];
	}
	//center
	for(i = 0; i < 640; i++){
		in_with_borders[i + (kernelSize-1)/2] = in[i];
	}
	//right edge 
	for(i = 0; i < (kernelSize-1)/2; i++){
		in_with_borders[i + (640 + (kernelSize - 1)/2)] = in[639];
	}
	out = (float*)realloc(out, sizeof(float) * (640 + (kernelSize -1)));
	//start convolution from out[kernelSize-1] to out[dataSize-1] (last)
	dataSize += kernelSize;
  for(i = kernelSize-1; i < (dataSize + (kernelSize-1)); ++i){
    out[i] = 0;                             // init to 0 before accumulate
    for(j = i, k = 0; k < kernelSize; --j, ++k){
      out[i] += in_with_borders[j] * kernel[k];
    }	            
  }
  //convolution from out[0] to out[kernelSize-2]
  for(i = 0; i < kernelSize - 1; ++i){
    out[i] = 0;                             // init to 0 before sum
    for(j = i, k = 0; j >= 0; --j, ++k){
      out[i] += in_with_borders[j] * kernel[k];
    }            
  }
	
	temp_float = (float*)calloc(sizeof(float), 640);
	for(i = 0; i < 640; i++){
		temp_float[i] = out[i + ((kernelSize - 1))];
	}
	out = (float*)realloc(out, sizeof(float)*640); 
	for(i = 0; i < 640; i++){
		out[i] = temp_float[i];
	}
  return 0;
}


float* Convolution::kernel1D(int width){
  float* kernel;
  int left_of_plus_one, right_of_plus_one, left_edge, right_edge;
  int j,k;
	//kernel[0] -> kernel[w*2]
	kernel = (float *)calloc(sizeof(float), width * 2 + 1); 
  //check if lane width is odd or even 
  if(isEven(width)){
    //width is even
    kernel[width] = 1;
    left_of_plus_one = right_of_plus_one = ((width / 2) - 1);
    k = width; 
    k = k + 1;
    for(j=0; j< right_of_plus_one; j++){
      kernel[k] = 1;
      k = k +1;
    }
    k = width;
    k = k -1;
    for(j=0; j< left_of_plus_one; j++){
      kernel[k] = 1;
      k = k -1;
    }
    int left_edge = ((width / 2) - 1); 
    int right_edge = ((width / 2) - 1); 
    for(j=0; j< left_edge; j++){
      kernel[j] = -1;
    }
    kernel[j] = -0.5; j++; kernel[j]=0; 
    k = width * 2 + 1;
    k = k-1;
    for(j=0; j < right_edge; j++){
      kernel[k] = -1; 
      k = k -1;
    }
    kernel[k] = -0.5; k--; kernel[k] = 0;
  }
  else{
    //width is odd
    kernel[width] = 1;
    left_of_plus_one = right_of_plus_one = floor(width / 2);
    k = width; 
    k = k + 1;
    for(j=0; j< right_of_plus_one; j++){
      kernel[k] = 1;
      k = k +1;
    }
    k = width;
    k = k -1;
    for(j=0; j< right_of_plus_one; j++){
      kernel[k] = 1;
      k = k -1;
    }
    left_edge = right_edge = floor(width / 2);
    for(j=0; j< left_edge; j++){
      kernel[j] = -1;
    }
    kernel[j]= -0.5; 
    k = width * 2 + 1;
    k = k -1;
    for(j=0; j < right_edge; j++){
      kernel[k] = -1; 
      k = k -1;
    }
    kernel[k] = -0.5;
  }
  return kernel;
}

bool Convolution::isEven(int width){
	return (width % 2 == 0) ? true : false; 
}

/* NOT BEING USED */
void Convolution::copyBorder(int* in, int kernel_width){
	int *temp = NULL;
	int i; 
	int new_row_width = (640 + 2*(kernel_width - 1));
	temp = (int*)calloc(sizeof(int), new_row_width);
	//left edge
	for(i = 0; i < (kernel_width-1)/2; i++){
		temp[i] = in[0];
	}
	//center
	
	for(i = 0; i < 640; i++){
		temp[i + (kernel_width-1)/2] = in[i];
	}
	//right edge 
	for(i = 0; i < (kernel_width-1)/2; i++){
		temp[i + (640 + (kernel_width - 1)/2)] = in[639];
	}

	printf("kernel_width = %d\n", kernel_width);
	in = (int*)realloc(in, sizeof(int) * (640 + (kernel_width -1)));
	for(i = 0; i < (640 + (kernel_width - 1)); i++){
		in[i] = temp[i]; 
	}
	
	free(temp); 
  return; 
}
