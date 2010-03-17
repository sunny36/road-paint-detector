#include "convolution.h"

void Convolution::convolve1D(std::vector<int> in, std::vector<float> kernel,
    std::vector<float>& out){
  int i, j, k;
  int new_row_width = (640 + (kernel.size() - 1));
  std::vector<int> in_with_borders(new_row_width);

  for (i = 0; i < ((kernel.size() - 1) / 2); i++){
    in_with_borders[i] = in[0];
  }
  for (i = 0; i < 640; i++) {
    in_with_borders[i + ((kernel.size() - 1) / 2)] = in[i];
  }
  for(i = 0; i < (kernel.size() - 1)/2; i++){
    in_with_borders[i + (640 + (kernel.size() - 1) / 2)] = in[639];
  }
  std::vector<float> out_with_borders(new_row_width);
  
  for(i = kernel.size() - 1; i < (in.size() + kernel.size() +  - 1); ++i){
    out_with_borders[i] = 0; // init to 0 before accumulate
    for(j = i, k = 0; k < kernel.size(); --j, ++k){
      out_with_borders[i] += in_with_borders[j] * kernel[k];
    }	            
  }

  for(i = 0; i < kernel.size() - 1; ++i){
    out_with_borders[i] = 0;  // init to 0 before sum
    for(j = i, k = 0; j >= 0; --j, ++k){
      out_with_borders[i] += in_with_borders[j] * kernel[k];
    }            
  }
  out.resize(640);
  for(i = 0; i < 640; i++){
    out[i] = out_with_borders[i + ((kernel.size() - 1))];

  }

  return;
}
  
void Convolution::kernel1D(int width, std::vector<float>& kernel){
  int kernel_size = width * 2 + 1;
  kernel.resize(kernel_size);

  int i, j;
  int left_edge, right_edge;

  for(i = 0; i < kernel.size(); i++){
    kernel[i] = 1;
  }

  if(isEven(width)){
    left_edge = right_edge = ((width / 2) - 1); 
    for (i = 0; i < left_edge; i++) {
      kernel[i] = -1;
    }
    kernel[i] = -0.5;
    i++;
    kernel[i] = 0; 
    j = kernel_size - 1; //last element of the kernel 
    for (i = 0; i < right_edge; i++) {
      kernel[j] = -1;
      j = j -1;
    }
    kernel[j] = -0.5;
    j--;
    kernel[j] = 0;
  }
  else{ // kernel is odd
    int left_edge = right_edge = floor(width / 2);
    for (i = 0; i < left_edge; i++) {
      kernel[i] = -1;
    }
    kernel[i] = -0.5; 
    j = kernel_size - 1; //last element of the kernel
    for (i = 0; i < right_edge; i++) {
      kernel[j] = -1;
      j = j -1;
    }
    kernel[j] = -0.5;
  }

  return;
}

bool Convolution::isEven(int width){
  return (width % 2 == 0) ? true : false; 
}

