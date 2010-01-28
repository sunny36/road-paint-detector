#include <cv.h>

class Camera{
  public:
    Camera();
    CvMat* getK();
    CvMat getT(CvMat* T); 
    CvMat getR(CvMat* R); 
    CvMat setP(CvMat* K, CvMat* R, CvMat* T); 
  
  private:
    CvMat *K;
    float degreesToRadians(float degrees);
};
