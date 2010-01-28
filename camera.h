#include <cv.h>

class Camera{
  public:
    Camera();

    void setK(float focal_lenght, int width, int height);
    CvMat* getK();

    CvMat* getT();

    void setR(float theta);
    CvMat* getR(); 

    CvMat setP(CvMat* K, CvMat* R, CvMat* T); 
  
  private:
    CvMat *K;
    CvMat *R;
    CvMat *T;
    float degreesToRadians(float degrees);
};
