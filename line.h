#include <cv.h> 
#include <vector> 
#include <iostream> 
#include <algorithm> 
#include <boost/tuple/tuple.hpp> 

class Line {
  friend class LineTest; 
 private:
  cv::Point2f normaized_vector; 
  cv::Point2f point_on_the_line; 
  std::vector<double> projected_points; 
  void calculate_projected_points(std::vector<cv::Point2f> points);
  std::vector<double> get_projected_points();
  double get_projected_PxPy(); 
  cv::Point2f get_max_point();
  cv::Point2f get_min_point();
 public:
  typedef boost::tuple<cv::Point, cv::Point> ExtremePoints; 
  void fit_line(std::vector<cv::Point2f> points); 
  void set_normalized_vector(double V_x, double V_y);
  void set_point_on_the_line(double P_x, double P_y);
  cv::Point2f get_normalized_vector();
  cv::Point2f get_point_on_the_line();
  ExtremePoints get_extreme_points(std::vector<cv::Point2f> points);
};
