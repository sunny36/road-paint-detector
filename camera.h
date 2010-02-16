#include <cv.h>
#include "io.h"

class Camera{
  public:
    Camera();
    void setK(float focal_lenght, int width, int height);
    void setR(float theta);
    void setT(float x, float y, float z);
    CvMat* getP(); 
    float calculateY(int current_row);
    CvPoint2D32f imageToGroundPlane(CvPoint pt);

  private:
    CvMat *K;
    CvMat *R;
    CvMat *T;
    CvMat *P;
    CvMat* getK();
    CvMat* getR(); 
    CvMat* getT();
    float degreesToRadians(float degrees);
};
