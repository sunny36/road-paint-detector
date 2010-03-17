#ifndef CONTOUR_H
#define CONTOUR_H

#include <highgui.h>
#include <cv.h>
#include "debug.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream> 
#include "camera.h"

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
    void printContours(CvSeq* contours, 
                       int number_of_contours,
                       std::string filename);
    void printContours(std::vector< std::vector<CvPoint2D32f> > contours, 
                       const std::string filename);

    void removeUnwantedContour();
    void removeDuplicatesInContours(CvSeq* contours);
    void scaleGroundPlaneSequences(
        std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences);

};

#endif
