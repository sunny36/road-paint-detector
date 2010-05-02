#ifndef UTIL_H
#define UTIL_H 

#include <cv.h> 
#include <highgui.h> 
#include "camera.h"

class Util { 
  public:
    void image_to_ground_plane(
      const std::vector< std::vector<CvPoint> >& image_plane_sequences,
      std::vector< std::vector<CvPoint2D32f> >& ground_plane_sequences,
      Camera& camera);
    void scale_ground_plane_sequences(
      std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences);
    void drawLines(std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences);
    void draw_ground_points(
      std::vector<std::vector<CvPoint2D32f> > ground_plane_sequences);
}; 



#endif
