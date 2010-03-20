#ifndef IO_H
#define IO_H

#include <cv.h>
#include <cstdio> 

class IO{
	public:
		void printMatrixFloat(CvMat* matrix, int rows, int cols);
};

#endif
