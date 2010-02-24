#ifndef CONTOUR_H
#define CONTOUR_H

#include <highgui.h>
#include <cv.h>
#include "debug.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream> 

class Contour{

  public:
    Contour();
    void findContours(IplImage* image);
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
    void removeUnwantedContour();
    void removeDuplicatesInContours(CvSeq* contours);
};

#endif
