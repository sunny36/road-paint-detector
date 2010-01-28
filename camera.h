#include <cv.h>

class Camera{
  public:
    Camera();
    CvMat* getK();
    CvMat* getT(); 
    CvMat* getR(); 
    CvMat setP(CvMat* K, CvMat* R, CvMat* T); 
  
  private:
    CvMat *K;
    CvMat *R;
    CvMat *T;
    float degreesToRadians(float degrees);
};
