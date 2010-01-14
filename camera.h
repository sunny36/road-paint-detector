#include <cv.h>

class Camera{
  public:
    CvMat getK(CvMat* K);
    CvMat getT(CvMat* T); 
    CvMat getR(CvMat* R); 
    CvMat setP(CvMat* K, CvMat* R, CvMat* T); 
  
  private:
    float degreesToRadians(float degrees);
};
