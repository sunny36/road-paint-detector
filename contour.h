#ifndef CONTOUR_H
#define CONTOUR_H

#include <highgui.h>
#include <cv.h>
#include "debug.h"
#include <string>
#include <vector>
#include <iostream> 
#include "camera.h"
#include "io.h"

class Contour{

  public:
    Contour();
    void findContours(IplImage* image, Camera camera);
    IplImage* drawContours();
    int getNumberOfContors();

  private:
    int number_of_contours;
  	CvMemStorage* storage;
	  CvSeq* contours; 
    IplImage* copied_image;
    void removeUnwantedContour();
    void removeDuplicatesInContours(CvSeq* contours);
    void scaleGroundPlaneSequences(std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences);
    void drawLines(std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences);
    void getImagePlaneSequences(CvSeq **contours, 
                                std::vector< std::vector <CvPoint> >& image_plane_sequences);
    void connectNearComponents(std::vector< std::vector<CvPoint> >& image_plane_sequences);
    CvPoint minY(std::vector<CvPoint> points);
    CvPoint maxY(std::vector<CvPoint> points);
};

#endif
