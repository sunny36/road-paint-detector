#include "contour.h"

Contour::Contour(){
  storage = cvCreateMemStorage(0);
}

void Contour::findContours(IplImage* image){
    //copy the image
    copied_image = cvCreateImage(cvGetSize(image), 8 , 1); // 8byte/pixel 1 channel
    cvCopy(image, copied_image);
    cvClearMemStorage( storage );
    number_of_contours = cvFindContours(  copied_image, 
                                          storage, 
                                          &contours, 
                                          sizeof(CvContour), 
                                          CV_RETR_LIST, 
                                          CV_CHAIN_APPROX_NONE	
                                        );
#if defined(DEBUG)
    printf("number of contours = %d\n", number_of_contours);
#endif
 
}

IplImage* Contour::drawContours(){
  cvDrawContours(copied_image, contours, cvScalar(255), cvScalar(255), 100); 
  return copied_image;
}

