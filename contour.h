#include <highgui.h>
#include <cv.h>
#include "debug.h"
#include <fstream>

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
    void printContours(CvSeq* contours);
};
