#include <highgui.h>
#include <cv.h>
#include "debug.h"

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
};
