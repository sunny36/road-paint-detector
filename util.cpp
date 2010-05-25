#include "util.h"

void Util::image_to_ground_plane(
    const std::vector< std::vector<CvPoint> >& image_plane_sequences,
    std::vector< std::vector<CvPoint2D32f> >& ground_plane_sequences, 
    Camera& camera) {
  int i, j; 
  for (i = 0; i < static_cast<int>(image_plane_sequences.size()); i++) {
    std::vector<CvPoint2D32f> sequence;
    for (j = 0; j < static_cast<int>(image_plane_sequences[i].size()); j++) {
      CvPoint2D32f ground_pt = camera.imageToGroundPlane(image_plane_sequences[i][j]); 
      sequence.push_back(ground_pt);
    }
    ground_plane_sequences.push_back(sequence);
  }

  return; 
}

void Util::scale_ground_plane_sequences(
    std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences){

  int i, j; 
  for (i = 0; i < static_cast<int>(ground_plane_sequences.size()); i++) {
    for (j = 0; j < static_cast<int>(ground_plane_sequences[i].size()); j++) {
      ground_plane_sequences[i][j].x = 320 + ground_plane_sequences[i][j].x * 10;
      ground_plane_sequences[i][j].y = 479 - ground_plane_sequences[i][j].y * 10;
    }
  }
}

void Util::draw_ground_points(
    std::vector<std::vector<CvPoint2D32f> > ground_plane_sequences) {
  IplImage* ground_points = cvCreateImage(cvSize(640, 480), 8, 1); 
  cvNamedWindow("ground_points", CV_WINDOW_AUTOSIZE); 
  cvZero(ground_points); 
  int i, j; 
  CvScalar value; 

  for (i = 0; i < ground_plane_sequences.size(); ++i) {
    for (j = 0; j < ground_plane_sequences[i].size(); ++j) {
      value.val[0] = 255; 
      if (cvRound(ground_plane_sequences[i][j].x) >= 0 && 
          cvRound(ground_plane_sequences[i][j].x) < 640 && 
          cvRound(ground_plane_sequences[i][j].y) >= 0 &&
          cvRound(ground_plane_sequences[i][j].y) < 480) {

        cvSet2D(ground_points, cvRound(ground_plane_sequences[i][j].y), 
                cvRound(ground_plane_sequences[i][j].x), value);  
      }
    }
  }
  cvShowImage("ground_points", ground_points); 

}

void Util::drawLines(std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences){

  IplImage* image_lines = cvCreateImage(cvSize(640, 480), 8, 3);
  cvNamedWindow("fitline", CV_WINDOW_AUTOSIZE);
  cvZero(image_lines); 
  int i, j;
  float line[4];
  CvPoint left, right; 

  for(i = 0; i < static_cast<int>(ground_plane_sequences.size()); i++){

    CvPoint* points = (CvPoint*)malloc(ground_plane_sequences[i].size() * sizeof(points[0]));
    CvMat pointMat = cvMat(1, ground_plane_sequences[i].size(), CV_32SC2, points); 

    for(j = 0; j < static_cast<int>(ground_plane_sequences[i].size()); j++){
      points[j].x = cvRound(ground_plane_sequences[i][j].x); 
      points[j].y = cvRound(ground_plane_sequences[i][j].y); 
    }
    cvFitLine(&pointMat, CV_DIST_L2, 1, 0.001, 0.001, line); 
    double a, b, c, d, e, f; 
    b = line[1] / line[0]; 
    a = line[3] - b*line[2]; 
    d = - 1/b; 
    c = points[0].y - d*points[0].x; 
    f = d; 
    e = points[ground_plane_sequences[i].size() - 1].y - 
        f*points[ground_plane_sequences[i].size() - 1].x; 
    left.x = (a - c) / (d - b); 
    left.y = c + d * left.x; 
    right.x = (a - e) / (f - b); 
    right.y = e + f * right.x; 

    //CvPoint center; 
    //center.x = line[2]; 
    //center.y = line[3]; 
    cvLine(image_lines, left, right, CV_RGB(255, 0, 0), 1, CV_AA, 0); 

    free(points); 
  }
  cvSaveImage("fit_line.png", image_lines);
  cvShowImage("fitline", image_lines); 
}

