#include "io.h"

void IO::printMatrixFloat(CvMat* matrix, int rows, int cols){
	int i, j;
	for(i = 0; i < rows; ++i){
		for(j =0; j < cols; ++j){
			fprintf(stderr, " %g\t", CV_MAT_ELEM(*matrix, float, i, j));
		}
		printf("\n");
	}
}

void IO::printContours(CvSeq* contours, 
                            const std::string filename){
    int i, n= 0;
    std::ofstream out(filename.c_str());
    out << "Number of contours = " << n << std::endl;
    for (CvSeq* c = contours; c!=NULL; c=c->h_next) {
      out << "Contour #" << n << std::endl;
      out << c->total << " elements:" << std::endl;
      for (i = 0; i < c->total; i++) {
        CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, c, i);        
        out << "(" << p->x << "," << p->y << ")" << std::endl; 
      }
     n++;
    }
    return;
}

void IO::printContours(std::vector< std::vector<CvPoint2D32f> > contours, 
    const std::string filename){
  int i, j = 0; 
  std::ofstream out(filename.c_str()); 
  out << "NUmber of contours = " << contours.size() << std::endl;
  for(i = 0; i < static_cast<int>(contours.size()); i++){
    out << "Contour #" << i << std::endl; 
    out << contours[i].size() << " elements:" << std::endl;
    for(j = 0; j < static_cast<int>(contours[i].size()); j++){
      out << "(" << contours[i][j].x << ", "  << contours[i][j].y << ")" 
        << std::endl; 
    }
    //std::cout << std::endl; 
  }
  return;
}

void IO::printContours(std::vector< std::vector<CvPoint> > contours, 
  const std::string filename){
  int i, j = 0; 
  std::ofstream out(filename.c_str()); 
  out << "NUmber of contours = " << contours.size() << std::endl;
  for(i = 0; i < static_cast<int>(contours.size()); i++){
    out << "Contour #" << i << std::endl; 
    out << contours[i].size() << " elements:" << std::endl;
    for(j = 0; j < static_cast<int>(contours[i].size()); j++){
      out << "(" << contours[i][j].x << ", "  << contours[i][j].y << ")" 
        << std::endl; 
    }
    //std::cout << std::endl; 
  }
  return;
}
