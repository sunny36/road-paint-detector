#include "main.h"

void mouseCallbackFunc(int event, int x, int y, int flags, void* param){
  if(event == CV_EVENT_LBUTTONDOWN){
    fprintf(stderr, "x = %d, y = %d\n", x, y);
  }
}
int main(int argc, char** argv){
  IplImage* img = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  cvNamedWindow("MIT Road Paint Detector", CV_WINDOW_AUTOSIZE); 
  cvSetMouseCallback("MIT Road Paint Detector", mouseCallbackFunc);

  /* camera matrix set up */
  Camera camera(FOCAL_LENGTH, 640, 480);
  CvMat *P = camera.getP();
  CvPoint2D32f point = camera.imageToGroundPlane(cvPoint(1,1));
  printf("x = %f, %f, \n", point.x, point.y);

  float Y;
  int row, col, width;
  std::vector<float> kernel;
  std::vector<float> out(640);
  std::vector<int> in(640);
  std::vector<float> normalized(640);
  std::vector<float> local_maxima(640);
  Convolution convolution;
  FILE *fp;
  int k;
  for (row = 0; row < img->height; row++) {
    Y = calculateY(P, row);
    width = calculateWidthInPixels(P, Y);
    if (width < ONE_PIXEL) {
      width = 0;
    }
#if defined(DEBUG)

    if (row == ROW_DEBUG) {
      fp = fopen("input.txt", "wt");
      fprintf(fp, "#\t X\t Y\n");
    }
#endif
    for (col = 0; col < img->width; col++) {
      CvScalar scalar = cvGet2D(img, row, col);
      in[col] = scalar.val[0];
#if defined(DEBUG)
      if (row == ROW_DEBUG) {
        fprintf(fp, "\t %d\t %d\t\n", col, in[col]);
      }
#endif
    }
#if defined(DEBUG)
    if (row == ROW_DEBUG) {
      fclose(fp);
    }
#endif
    //find the kernel only if only there is a width
    if (width > 0) {
      convolution.kernel1D(width, kernel);
      convolution.convolve1D(in, kernel, out);
#if defined(DEBUG)
      if (row == ROW_DEBUG) {
        //convolution result
        fp = fopen("convolution.txt", "wt"); 
        fprintf(fp, "#\t X\t Y\n");
        for (col = 0; col < img->width; col++) {
          fprintf(fp, "\t %d\t %f\n", col, out[col]);
        }
        fclose(fp);

        //kernel result
        fp = fopen("kernel.txt", "wt"); 
        fprintf(fp, "#\t X\t Y\n");
        int left = in.size() - kernel.size() / 2;
        int right = left + 1;
        for (col = 0; col < left; col++) {
          fprintf(fp, "\t %d\t %f\n", col, 0.0);
        }
        k = col;
        for (col = 0; col < kernel.size(); col++) {
          fprintf(fp, "\t %d\t %f\n", k++, kernel[col] * 255);
        }
        for (col = 0; col < right; col++) {
          fprintf(fp, "\t %d\t %f\n", k++, 0.0);
        } 
        fclose(fp);
      }
#endif
      convolution.normalization(out, normalized, img->width, width);
#if defined(DEBUG)
      if(row == ROW_DEBUG) {
        fp = fopen("normalization.txt", "wt");
        fprintf(fp, "#\t X\t Y\n");
        for (col = 0; col < img->width; col++) {
          fprintf(fp, "\t %d\t %f\n", col, normalized[col]);
        }
        fclose(fp);
      }
#endif
      convolution.localMaximaSuppression(normalized, local_maxima);
#if defined(DEBUG)
      if(row == ROW_DEBUG) {
        fp = fopen("localmaxima.txt", "wt");
        fprintf(fp, "#\t X\t Y\n");
        for (col = 0; col < img->width; col++) {
          fprintf(fp, "\t %d\t %f\n", col, local_maxima[col]);
        }
        fclose(fp);
      }
#endif

    }
    else {
      local_maxima.resize(img->width); 
      for (col = 0; col < img->width; col++) {
        local_maxima[col] = 0;
      }
    }
    for (col = 0; col < img->width; col++) {
      CvScalar scalar;
      if (local_maxima[col] > 0) {
        local_maxima[col] = 255;
      }
      scalar.val[0] = local_maxima[col];
      cvSet2D(img, row, col, scalar);
    }
  } // end for loop

  cvSaveImage("localmaxima.png", img);

  ConnectedComponent connected_component; 
  ConnectedComponent::Algorithm algorithm = ConnectedComponent::NINE_POINT; 
  connected_component.setImage(img, algorithm); 
  connected_component.runPass1(); 
  connected_component.runPass2(); 
  std::vector<std::vector<CvPoint> > image_plane_sequences;
  image_plane_sequences = connected_component.get_connected_component(); 

  Util util; 
  std::vector<std::vector<CvPoint2D32f> > ground_plane_sequences;
  util.image_to_ground_plane(image_plane_sequences, 
                             ground_plane_sequences,
                             camera); 
  IO io; 
  io.printContours(ground_plane_sequences, "ground_planes_sequences"); 

  util.scale_ground_plane_sequences(ground_plane_sequences); 
  io.printContours(ground_plane_sequences, "scale_ground_plane_sequences"); 
  util.drawLines(ground_plane_sequences); 
  util.draw_ground_points(ground_plane_sequences); 

  cvShowImage("MIT Road Paint Detector", img);     

  cvWaitKey(0);
  cvReleaseImage(&img);
  cvDestroyWindow("MIT Read Paint Detector");
  return 0;
}

int calculateWidthInPixels(CvMat* P, float Y){
  float W = 0.10; //width of road 20cm ~ 0.2m 
  float w = 0.0; //width of the roads in pixels

  CvMat tmp;
  //create P_1 (row 1 of matrix P)
  CvMat *P_1 = cvCreateMat(1,4,CV_32FC1);
  cvGetRow(P,&tmp,0); //row 0
  cvCopy(&tmp,P_1,NULL);

  CvMat *P_3 = cvCreateMat(1,4,CV_32FC1);
  cvGetRow(P,&tmp,2); //row 2
  cvCopy(&tmp,P_3,NULL);

  CvMat* X_1 = cvCreateMat(4,1,CV_32FC1);
  CvMat* X_2 = cvCreateMat(4,1,CV_32FC1);
  CvMat* P_1_times_X_1 = cvCreateMat(1,1,CV_32FC1);
  CvMat* P_3_times_X_1 = cvCreateMat(1,1,CV_32FC1);
  CvMat* P_1_times_X_2 = cvCreateMat(1,1,CV_32FC1);
  CvMat* P_3_times_X_2 = cvCreateMat(1,1,CV_32FC1);

  cvmSet(X_1,0,0,W);
  cvmSet(X_1,1,0,Y);
  cvmSet(X_1,2,0,0.0);
  cvmSet(X_1,3,0,1.0);

  cvmSet(X_2,0,0,0);
  cvmSet(X_2,1,0,Y);
  cvmSet(X_2,2,0,0);
  cvmSet(X_2,3,0,1);

  cvMatMul(P_1,X_1,P_1_times_X_1);
  cvMatMul(P_3,X_1,P_3_times_X_1);	
  cvMatMul(P_1,X_2,P_1_times_X_2);
  cvMatMul(P_3,X_2,P_3_times_X_2);	

  w = ((cvmGet(P_1_times_X_1,0,0) /
        cvmGet(P_3_times_X_1,0,0)
       ) 
      -
      (cvmGet(P_1_times_X_2,0,0) /
       cvmGet(P_3_times_X_2,0,0)
      )); 


  return round(w);
}

float calculateY(CvMat* P, int current_row){
  float Y=0, P_34=0, P_24=0, P_32=0, P_22=0;
  /*
     Y = (v*P34 - P24) / (v*P32 - P22)
     */
  P_34 = cvmGet(P, 2,3);
  P_24 = cvmGet(P, 1,3);
  P_32 = cvmGet(P, 2,1);
  P_22 = cvmGet(P, 1,1);
  Y = (current_row*P_34 - P_24) / (P_22 - current_row*P_32);		
  return Y;
}


