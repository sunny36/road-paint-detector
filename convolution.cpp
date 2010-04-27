#include "convolution.h"

void Convolution::convolve1D(std::vector<int> in, std::vector<float> kernel,
    std::vector<float>& out){
  int i, j, k;
  int new_row_width = (640 + (kernel.size() - 1));
  std::vector<int> in_with_borders(new_row_width);

  for (i = 0; i < ((static_cast<int>(kernel.size()) - 1) / 2); i++){
    in_with_borders[i] = in[0];
  }
  for (i = 0; i < 640; i++) {
    in_with_borders[i + ((static_cast<int>(kernel.size())- 1) / 2)] = in[i];
  }
  for(i = 0; i < (static_cast<int>(kernel.size()) - 1)/2; i++){
    in_with_borders[i + (640 + (static_cast<int>(kernel.size()) - 1) / 2)] = in[639];
  }
  std::vector<float> out_with_borders(new_row_width);
  
  for(i = static_cast<int>(kernel.size()) - 1; 
      i < (static_cast<int>(in.size()) + static_cast<int>(kernel.size()) +  - 1);
      ++i){
    out_with_borders[i] = 0; // init to 0 before accumulate
    for(j = i, k = 0; k < static_cast<int>(kernel.size()); --j, ++k){
      out_with_borders[i] += in_with_borders[j] * kernel[k];
    }	            
  }

  for(i = 0; i < static_cast<int>(kernel.size()) - 1; ++i){
    out_with_borders[i] = 0;  // init to 0 before sum
    for(j = i, k = 0; j >= 0; --j, ++k){
      out_with_borders[i] += in_with_borders[j] * kernel[k];
    }            
  }
  out.resize(640);
  for(i = 0; i < 640; i++){
    out[i] = out_with_borders[i + ((static_cast<int>(kernel.size()) - 1))];

  }

  return;
}
  
void Convolution::kernel1D(int width, std::vector<float>& kernel){
  int kernel_size = width * 2 + 1;
  kernel.resize(kernel_size);

  int i, j;
  int left_edge, right_edge;

  for(i = 0; i < static_cast<int>(kernel.size()); i++){
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

void Convolution::normalization(const std::vector<float> out, 
                                std::vector<float>& normalized,
                                int n, int lane_width){
  float max = 0.0;
  int i; 
  max = 255 * lane_width;
  float cut_off = 0.15 *  max; 
  for(i = 0; i < n; i++){
    (out[i] < cut_off) ? normalized[i] = 0.0 : normalized[i] = out[i];
  }
  for(i = 0; i < n; i++){
    normalized[i] = (normalized[i] / (255 * lane_width)) * 255;
  }
  return;
}

void Convolution::localMaximaSuppression(const std::vector<float> image_row,
                                         std::vector<float>& local_maxima) {

  int i; 
  for (i = 0; i < static_cast<int>(image_row.size()); ++i) { 
    local_maxima[i] = 0.0; 
  }
  for(i = 0; i < static_cast<int>(image_row.size()); i++){
    //first pixel
    if(i == 0){ 
      if(image_row[i] > image_row[i+1]){
        local_maxima[i] = image_row[i];
        local_maxima[i+1] = 0.0;
      }
      else if(image_row[i] < image_row[i+1]){
        local_maxima[i] = 0.0;
        local_maxima[i+1] = image_row[i];
      }	
    } 

    //pixels between first and last 
    if(i > 0 && i <  static_cast<int>(image_row.size())- 1){
     if (image_row[i] == image_row[i+1]) { 
        int k = i + 1; 
        while(image_row[k] == image_row[i]) { 
          k++; 
        }
        int number_of_pixels = 0; 
        int sum_of_pixels = 0;
        if(image_row[k] < image_row[i]) { 
          for (int j = i; j < k - 1; ++j) { 
            sum_of_pixels += j;
            number_of_pixels++;
          }
          int center_location = sum_of_pixels / number_of_pixels; 
          local_maxima[center_location] = image_row[i]; 
          for (int j = i; j < center_location; ++j) { 
            local_maxima[j] = 0; 
          }
          i = k - 1; //reset i
        }
     }
      if(image_row[i] > image_row[i-1] && image_row[i] > image_row[i+1]){
        local_maxima[i] = image_row[i];
        local_maxima[i-1] = 0.0;
        local_maxima[i+1] = 0.0;
      }
      if(image_row[i] < image_row[i-1] && image_row[i] < image_row[i+1]){
        local_maxima[i] = 0.0;
        local_maxima[i-1] = image_row[i];
        local_maxima[i+1] = image_row[i];
      }
    }

    //last pixel
    if( i == static_cast<int>(image_row.size()) - 1 ){
      if(image_row[i] > image_row[i-1]){
        local_maxima[i] = image_row[i];
      }
      else if(image_row[i] < image_row[i-1]){
        local_maxima[i] = 0.0;
      }				
    }

  }

  return; 
}


