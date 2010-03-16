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
  int row, col, j, k, width;
  std::vector<float> kernel;
  std::vector<float> out(640);
  std::vector<int> in(640);
  Convolution convolution;
  FILE *fp;
  for (row = 0; row < img->height; row++) {
    Y = calculateY(P, row);
    width = calculateWidthInPixels(P, Y);
    if (width < ONE_PIXEL) {
      width = 0;
    }
#if defined(DEBUG)
    if (row == 374) {
      fp = fopen("input.txt", "wt");
      fprintf(fp, "#\t X\t Y\n");
    }
#endif
    for (col = 0; col < img->width; col++) {
      CvScalar scalar = cvGet2D(img, row, col);
      in[col] = scalar.val[0];
#if defined(DEBUG)
      if (row == 374) {
        fprintf(fp, "\t %d\t %d\t\n", col, in[col]);
      }
#endif
    }
#if defined(DEBUG)
    if (row == 374) {
      fclose(fp);
    }
#endif
    //find the kernel only if only there is a width
    if (width > 0) {
      convolution.kernel1D(width, kernel);
      convolution.convolve1D(in, kernel, out);
#if defined(DEBUG)
      if (row == 374) {
        fp = fopen("convolution.txt", "wt"); 
        for (col = 0; col < img->width; col++) {
          fprintf(fp, "\t %d\t %f\n", col, out[col]);
        }
        fclose(fp);
      }
#endif
      normalization(out, img->width, width);
      localMaximaSuppression(out, img->width);
    }
    for (col = 0; col < img->width; col++) {
      CvScalar scalar;
      scalar.val[0] = out[col];
      cvSet2D(img, row, col, scalar);
    }
  } // for each row
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

void localMaximaSuppression(std::vector<float>& image_row, int row_size){
  int i; 
  float* image_row_suppressed;
  image_row_suppressed = (float*)calloc(sizeof(float), row_size);

  for(i = 0; i < row_size; i++){
    //first pixel
    if(i == 0){ 
      if(image_row[i] > image_row[i+1]){
        image_row_suppressed[i] = image_row[i];
        image_row_suppressed[i+1] = 0.0;
      }
      else if(image_row[i] < image_row[i+1]){
        image_row_suppressed[i] = 0.0;
        image_row_suppressed[i+1] = image_row[i];
      }	
    } 

    //pixels between first and last 
    if(i > 0 && i < row_size - 1){
      if(image_row[i] > image_row[i-1] && image_row[i] > image_row[i+1]){
        image_row_suppressed[i] = image_row[i];
        image_row_suppressed[i-1] = 0.0;
        image_row_suppressed[i+1] = 0.0;
      }
      if(image_row[i] < image_row[i-1] && image_row[i] < image_row[i+1]){
        image_row_suppressed[i] = 0.0;
        image_row_suppressed[i-1] = image_row[i];
        image_row_suppressed[i+1] = image_row[i];
      }
    }

    //last pixel
    if( i == row_size - 1 ){
      if(image_row[i] > image_row[i-1]){
        image_row_suppressed[i] = image_row[i];
      }
      else if(image_row[i] < image_row[i-1]){
        image_row_suppressed[i] = 0.0;
      }				
    }

  }

  //copy value from image_row_suppressed to image_row
  for(i = 0; i < row_size; i++){
    image_row[i] = image_row_suppressed[i];
  }
  free(image_row_suppressed);
  return; 
}

void normalization(std::vector<float>& out, int n, int lane_width){
  float max = 0.0;
  int i; 
  for(i = 0; i < n; i++){
    (out[i] > max) ? max = out[i] : max = max;
  }
  float cut_off = 0.9 * max; 
  for(i = 0; i < n; i++){
    (out[i] < cut_off) ? out[i] = 0.0 : out[i] = out[i];
  }
  for(i = 0; i < n; i++){
    out[i] = (out[i] / (255 * lane_width)) * 255;
  }
}

void plotGraph(const char* filename){
  FILE *pipe = popen("gnuplot -persist","w");

  fprintf(pipe, "plot '%s' with lines", filename);
  pclose(pipe); 
}



