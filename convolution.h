#include <stdlib.h>
#include <math.h>
#include <highgui.h>
#include <cv.h>

class Convolution{
  public:
      int convolve1D( int* in, 
                      float* out, 
                      int dataSize, 
                      float* kernel, 
                      int kernelSize);
			float* kernel1D(int w);
  private:
      void copyBorder(int* in, int kernel_width);
};

