#include <limits.h>
#include "../convolution.h"
#include <gtest/gtest.h>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h> 
#include <iostream>

// Tests Factorial().

// Tests factorial of negative numbers.
TEST(kernel1D, LaneWidthIsEven) {
  
  float data[] = { -1, -0.5, 0, 1, 1, 1, 0, -0.5, -1 };
  std::vector<float> expected_kernel(data, data + sizeof(data)/sizeof(float));

  Convolution convolution;
  std::vector<float> actual_kernel;
  convolution.kernel1D(4, actual_kernel);
  int i; 
  for(i = 0; i < expected_kernel.size(); i++){
   EXPECT_EQ(expected_kernel[i], actual_kernel[i]);
  }
}

TEST(convolution1D, ConvolutionIsCorrect){
  int in[] = {3, 4, 5, 0, 0};
  float kernel[] = { -1, -0.5, 0, 1, 1, 1, 0, -0.5, -1 };
