#include <limits.h>
#include "camera.h"
#include "cv.h"
#include <gtest/gtest.h>
<<<<<<< HEAD
// Tests Factorial().

// Tests factorial of negative numbers.
TEST(CameraMatrix, K_IsSetCorrectly) {

  float FOCAL_LENGTH = 1563.0; 
 	Camera camera(FOCAL_LENGTH, 640, 480);
	CvMat *K = camera.getK();
	
	//K should have 3 rows and 3 cols
	EXPECT_EQ(K->rows, 3);
	EXPECT_EQ(K->rows, 3);
	//Test all values in the camera matrix
	EXPECT_EQ(cvmGet(K, 0, 0), FOCAL_LENGTH);
	EXPECT_EQ(cvmGet(K, 0, 1), 0);
	EXPECT_EQ(cvmGet(K, 0, 2), 640/2);
	
	EXPECT_EQ(cvmGet(K, 1, 0), 0);
	EXPECT_EQ(cvmGet(K, 1, 1), FOCAL_LENGTH);
	EXPECT_EQ(cvmGet(K, 1, 2), 480/2);
	
	EXPECT_EQ(cvmGet(K, 2, 0), 0);
	EXPECT_EQ(cvmGet(K, 2, 1), 0);
	EXPECT_EQ(cvmGet(K, 2, 2), 1);
=======

class CameraTest : public ::testing::Test{
  protected:
    void TestRTcw(){
      float FOCAL_LENGTH = 1563.0; 
      Camera camera(FOCAL_LENGTH, 640, 480);
      CvMat *RTcw = camera.getRTcw();

      //Rcw should have 3 rows and 3 cols
      EXPECT_EQ(RTcw->rows, 4);
      EXPECT_EQ(RTcw->cols, 4);

      //Test all values in the camera matrix
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 0), 1.0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 1), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 2), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 3), 0);

      EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 0), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 1), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 2), 1);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 3), 2.15);

      EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 0), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 1), -1);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 2), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 3), 1);

      EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 0), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 1), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 2), 0);
      EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 3), 1);

    }
};

// Tests factorial of negative numbers.
TEST_F(CameraTest, K_IsSetCorrectly) {
  float FOCAL_LENGTH = 1563.0; 
  Camera camera(FOCAL_LENGTH, 640, 480);
  CvMat *K = camera.getK();

  //K should have 3 rows and 3 cols
  EXPECT_EQ(K->rows, 3);
  EXPECT_EQ(K->rows, 3);
  //Test all values in the camera matrix
  EXPECT_EQ(cvmGet(K, 0, 0), FOCAL_LENGTH);
  EXPECT_EQ(cvmGet(K, 0, 1), 0);
  EXPECT_EQ(cvmGet(K, 0, 2), 640/2);

  EXPECT_EQ(cvmGet(K, 1, 0), 0);
  EXPECT_EQ(cvmGet(K, 1, 1), FOCAL_LENGTH);
  EXPECT_EQ(cvmGet(K, 1, 2), 480/2);

  EXPECT_EQ(cvmGet(K, 2, 0), 0);
  EXPECT_EQ(cvmGet(K, 2, 1), 0);
  EXPECT_EQ(cvmGet(K, 2, 2), 1);

>>>>>>> 31726e09faae4be7cbb17deaf97cfbad0140f9bf
}

TEST(CameraMatrix, Rcw_IsSetCorretly) {

  float FOCAL_LENGTH = 1563.0; 
 	Camera camera(FOCAL_LENGTH, 640, 480);
	CvMat *Rcw = camera.getRcw();
	
	//Rcw should have 3 rows and 3 cols
	EXPECT_EQ(Rcw->rows, 3);
	EXPECT_EQ(Rcw->rows, 3);
	
	//Test all values in the camera matrix
	EXPECT_EQ(cvmGet(Rcw, 0, 0), 1);
	EXPECT_EQ(cvmGet(Rcw, 0, 1), 0);
	EXPECT_EQ(cvmGet(Rcw, 0, 2), 0);
	
	EXPECT_EQ(cvmGet(Rcw, 1, 0), 0);
	EXPECT_EQ(cvmGet(Rcw, 1, 1), 0);
	EXPECT_EQ(cvmGet(Rcw, 1, 2), 1);
	
	EXPECT_EQ(cvmGet(Rcw, 2, 0), 0);
	EXPECT_EQ(cvmGet(Rcw, 2, 1), -1);
	EXPECT_EQ(cvmGet(Rcw, 2, 2), 0);
}

TEST(CameraMatrix, Tcw_IsSetCorretly) {

  float FOCAL_LENGTH = 1563.0; 
 	Camera camera(FOCAL_LENGTH, 640, 480);
	CvMat *Tcw = camera.getTcw();
	
	//Rcw should have 3 rows and 3 cols
	EXPECT_EQ(Tcw->rows, 3);
	EXPECT_EQ(Tcw->cols, 1);
	
	//Test all values in the camera matrix
	EXPECT_FLOAT_EQ(cvmGet(Tcw, 0, 0), 0);
	EXPECT_FLOAT_EQ(cvmGet(Tcw, 1, 0), 2.15);
	EXPECT_FLOAT_EQ(cvmGet(Tcw, 2, 0), 1.0);
}

<<<<<<< HEAD
TEST(CameraMatrix, RTcw_IsSetCorretly) {

  float FOCAL_LENGTH = 1563.0; 
 	Camera camera(FOCAL_LENGTH, 640, 480);
	CvMat *RTcw = camera.getRTcw();
	
	//Rcw should have 3 rows and 3 cols
	EXPECT_EQ(RTcw->rows, 4);
	EXPECT_EQ(RTcw->cols, 4);
	
	//Test all values in the camera matrix
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 0), 1.0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 1), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 2), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 0, 3), 0);
	
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 0), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 1), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 2), 1);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 1, 3), 2.15);

	EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 0), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 1), -1);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 2), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 2, 3), 1);

	EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 0), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 1), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 2), 0);
	EXPECT_FLOAT_EQ(cvmGet(RTcw, 3, 3), 1);
	
=======
TEST_F(CameraTest, RTcw_IsSetCorretly) {
  TestRTcw();	
>>>>>>> 31726e09faae4be7cbb17deaf97cfbad0140f9bf
}

TEST(CameraMatrix, P_IsSetCorretly) {

  float FOCAL_LENGTH = 1563.0; 
 	Camera camera(FOCAL_LENGTH, 640, 480);
	CvMat *P = camera.getP();
	
	//Rcw should have 3 rows and 3 cols
	EXPECT_EQ(P->rows, 3);
	EXPECT_EQ(P->cols, 4);
	
	//Test all values in the camera matrix
	EXPECT_FLOAT_EQ(cvmGet(P, 0, 0), 1563);
	EXPECT_FLOAT_EQ(cvmGet(P, 0, 1), 320);
	EXPECT_FLOAT_EQ(cvmGet(P, 0, 2), 0);
	EXPECT_FLOAT_EQ(cvmGet(P, 0, 3), -688);
	
	EXPECT_FLOAT_EQ(cvmGet(P, 1, 0), 0);
	EXPECT_FLOAT_EQ(cvmGet(P, 1, 1), 240);
	EXPECT_FLOAT_EQ(cvmGet(P, 1, 2), -1563);
	EXPECT_FLOAT_EQ(cvmGet(P, 1, 3), 1047);
	
	EXPECT_FLOAT_EQ(cvmGet(P, 2, 0), 0);
	EXPECT_FLOAT_EQ(cvmGet(P, 2, 1), 1);
	EXPECT_FLOAT_EQ(cvmGet(P, 2, 2), 0);
	EXPECT_FLOAT_EQ(cvmGet(P, 2, 3), -2.15);
}

