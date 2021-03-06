#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdlib.h>
#include <highgui.h>
#include <cv.h>
#include <vector>
#include <cmath>
#include <iostream> 

class Convolution{
  public:
      void convolve1D(std::vector<int> in, std::vector<float> kernel,
                      std::vector<float>& out);
      void kernel1D(int width, std::vector<float>& kernel);
      void localMaximaSuppression(const std::vector<float> image_row,
                                  std::vector<float>& local_maxima);
      void normalization(const std::vector<float> out, 
                         std::vector<float>& normalized,int n, int lane_width);

 private:
		bool isEven(int width);
};

#endif
