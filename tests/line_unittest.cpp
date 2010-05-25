#include "../line.h" 
#include <cv.h> 
#include <gtest/gtest.h> 

class LineTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
      points_.push_back(cv::Point2f(1, 1)); 
      points_.push_back(cv::Point2f(3, 3)); 
      points_.push_back(cv::Point2f(10,10)); 
      line.fit_line(points_); 
    }
    void calculate_projected_points(Line *obj) {
      obj->calculate_projected_points(points_); 
    }
    std::vector<double> get_projected_points(Line* obj) {
      return obj->get_projected_points(); 
    }
    double get_projected_PxPy(Line* obj) {
      return obj->get_projected_PxPy(); 
    }
    cv::Point2f get_max_point(Line* obj) {
      return obj->get_max_point(); 
    }
    cv::Point2f get_min_point(Line* obj) {
      return obj->get_min_point(); 
    }
    std::vector<cv::Point2f> points_; 
    Line line; 
};

TEST_F(LineTest, TestFitLine) { 
  EXPECT_NEAR(0.707, line.get_normalized_vector().x, 0.001);
  EXPECT_NEAR(0.707, line.get_normalized_vector().y, 0.001);
  EXPECT_NEAR(4.666, line.get_point_on_the_line().x, 0.001);
  EXPECT_NEAR(4.666, line.get_point_on_the_line().y, 0.001);
}

TEST_F(LineTest, TestProjectedPoints) { 
  calculate_projected_points(&line); 
  std::vector<double> projected_points = get_projected_points(&line); 
    EXPECT_NEAR(1.4142, projected_points[0], 0.001); 
    EXPECT_NEAR(4.2426, projected_points[1], 0.001); 
    EXPECT_NEAR(14.1421, projected_points[2], 0.001); 
}

TEST_F(LineTest, TestProjectedPxPy) {
  EXPECT_NEAR(6.59966, get_projected_PxPy(&line), 0.001); 
}


TEST_F(LineTest, TestExtremePoints) {
  cv::Point2f min_point; 
  cv::Point2f max_point;
  Line::ExtremePoints extreme_points = (*this).line.get_extreme_points
      ((*this).points_);
  boost::tie(min_point, max_point) =  extreme_points; 
  EXPECT_EQ(1, min_point.x); 
  EXPECT_EQ(1, min_point.y); 
  EXPECT_EQ(10, max_point.x); 
  EXPECT_EQ(10, max_point.y); 
}
