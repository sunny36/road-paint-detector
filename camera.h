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
		CvMat* getRTcw();
    float calculateY(int current_row);
    //CvPoint2D32f imageToGroundPlane(CvPoint pt);

  private:
    CvMat *K;
    CvMat *Rcw;
    CvMat *Tcw;
    CvMat *P;
<<<<<<< HEAD
=======
    CvMat* getRTcw();
>>>>>>> 31726e09faae4be7cbb17deaf97cfbad0140f9bf
    float degreesToRadians(float degrees);
    void setK(float focal_lenght, int width, int height);
    void setRcw(float theta);
    void setTcw(float x, float y, float z);

};

#endif
