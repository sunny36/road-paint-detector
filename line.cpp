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

void Line::calculate_projected_points(std::vector<cv::Point2f> points) {
  for (int i = 0; i < static_cast<int>(points.size()); ++i) {
    (*this).projected_points.push_back(points[i].dot((*this).normaized_vector)); 
  }
}

std::vector<double> Line::get_projected_points() {
  return (*this).projected_points;
}

double Line::get_projected_PxPy() {
  return (*this).point_on_the_line.dot((*this).normaized_vector); 
}

cv::Point2f Line::get_max_point() {
  std::sort(projected_points.begin(), projected_points.end()); 
  double projected_max = projected_points[projected_points.size() - 1];
  double distance_max = projected_max - (*this).get_projected_PxPy(); 
  cv::Point2f point =  (*this).get_point_on_the_line() + 
      (distance_max * (*this).get_normalized_vector()); 
  return point; 
}

cv::Point2f Line::get_min_point() {
  std::sort(projected_points.begin(), projected_points.end()); 
  double projected_min = projected_points[0];
  double distance_min = projected_min - (*this).get_projected_PxPy(); 
  cv::Point2f point =  (*this).get_point_on_the_line() + 
      (distance_min * (*this).get_normalized_vector()); 
  return point; 
}

Line::ExtremePoints Line::get_extreme_points(std::vector<cv::Point2f> points) {
  (*this).calculate_projected_points(points); 
  //Round off the points
  cv::Point min_point(cvRound((*this).get_min_point().x), 
                      cvRound((*this).get_min_point().y)), 
            max_point(cvRound((*this).get_max_point().x), 
                      cvRound((*this).get_max_point().y));
  return boost::make_tuple(min_point, max_point); 
}

