#include "../line.h" 
#include <cv.h> 
#include <gtest/gtest.h> 

TEST(fit_line, TestFitLine) { 
  std::vector<cv::Point2f> points; 
  points.push_back(cv::Point2f(1, 1)); 
  points.push_back(cv::Point2f(3, 3)); 
  points.push_back(cv::Point2f(10,10)); 
  Line line; 
  line.fit_line(points); 
  EXPECT_NEAR(0.707, line.get_normalized_vector().x, 0.001);
  EXPECT_NEAR(0.707, line.get_normalized_vector().y, 0.001);
  EXPECT_NEAR(4.666, line.get_point_on_the_line().x, 0.001);
  EXPECT_NEAR(4.666, line.get_point_on_the_line().y, 0.001);
}
