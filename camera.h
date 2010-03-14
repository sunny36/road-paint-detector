#ifndef CAMERA_H
#define CAMERA_H

#include <cv.h>
#include "io.h"

class Camera{
  friend class CameraTest;
  public:
    Camera(float focal_lenght, int width, int height);
    CvMat* getK();
    CvMat* getRcw(); 
    CvMat* getTcw();
    CvMat* getP(); 
    float calculateY(int current_row);
    CvPoint2D32f imageToGroundPlane(CvPoint pt);

  private:
    CvMat *K;
    CvMat *Rcw;
    CvMat *Tcw;
    CvMat *P;
    CvMat* getRTcw();    
    float degreesToRadians(float degrees);
    void setK(float focal_lenght, int width, int height);
    void setRcw(float theta);
    void setTcw(float x, float y, float z);

};

#endif
