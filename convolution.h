#include <stdlib.h>
#include <math.h>
#include <highgui.h>
#include <cv.h>

class Convolution{
  public:
      int convolve1D(int* in, float* out, int dataSize, float* kernel, 
										 int kernelSize);
			float* kernel1D(int w);
  private:
		// const int PLUS_ONE = 1; 
		// const int MINUS_ONE = -1; 
		// const float PLUS_ZERO_POINT_FIVE = 0.5; 
		// const float MINUS_ZERO_POINT_FIVE = 0.5; 
    void copyBorder(int* in, int kernel_width);
		bool isEven(int width);
};

