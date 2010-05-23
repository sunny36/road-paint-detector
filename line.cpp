#include "line.h"

void Line::fit_line(std::vector<cv::Point2f> points) {
  cv::Vec4f line;
  cv::Mat point_matrix_format = cv::Mat(points); 
  cv::fitLine(cv::Mat(points), line, CV_DIST_L2, 0.001, 0.001, 0.01); 
  (*this).set_normalized_vector(line[0], line[1]); //V_x, V_y
  (*this).set_point_on_the_line(line[2], line[3]); //P_x, P_y
  return;
}

void Line::set_normalized_vector(double V_x, double V_y) {
  (*this).normaized_vector.x = V_x; 
  (*this).normaized_vector.y = V_y; 
}

void Line::set_point_on_the_line(double P_x, double P_y) {
  (*this).point_on_the_line.x = P_x; 
  (*this).point_on_the_line.y = P_y; 
}

cv::Point2f Line::get_normalized_vector() {
  return normaized_vector; 
}

cv::Point2f Line::get_point_on_the_line() {
  return point_on_the_line; 
}
