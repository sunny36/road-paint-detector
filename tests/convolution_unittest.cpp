#include <limits.h>
#include "convolution.h"
#include <gtest/gtest.h>
#include <cv.h>
#include <highgui.h>
// Tests Factorial().

// Tests factorial of negative numbers.
TEST(kernel1D, LaneWidthIsEven) {
  
  float expected_kernel[] = { -1, -0.5, 0, 1, 1, 1, 0, -0.5, -1 };
  int expected_kernel_size = sizeof(expected_kernel)/sizeof(expected_kernel[0]);
  Convolution convolution; 
  float* actual_kernel = convolution.kernel1D(4);
  int i; 
  for(i = 0; i < expected_kernel_size; i++){
    EXPECT_EQ(expected_kernel[i], actual_kernel[i]);
  }
}

