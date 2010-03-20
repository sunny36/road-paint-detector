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
  private:
		bool isEven(int width);
};

#endif
