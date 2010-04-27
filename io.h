#ifndef IO_H
#define IO_H

#include <cv.h>
#include <cstdio> 
#include <fstream>

class IO{
	public:
		void printMatrixFloat(CvMat* matrix, int rows, int cols);
    void printContours(CvSeq* contours, 
                       std::string filename);
    void printContours(std::vector< std::vector<CvPoint2D32f> > contours, 
                       const std::string filename);
    void printContours(std::vector< std::vector<CvPoint> > contours, 
                       const std::string filename);

};

#endif
