#include <cv.h> 
#include <vector> 

class Line {
  private:
    cv::Point2f normaized_vector; 
    cv::Point2f point_on_the_line; 
  public:
    void fit_line(std::vector<cv::Point2f> points); 
    void set_normalized_vector(double V_x, double V_y);
    void set_point_on_the_line(double P_x, double P_y);
    cv::Point2f get_normalized_vector();
    cv::Point2f get_point_on_the_line();
};
