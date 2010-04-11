#include "contour.h"

Contour::Contour(){
  storage = cvCreateMemStorage(0);
}

void Contour::findContours(IplImage* image, Camera camera){
    //copy the image
    copied_image = cvCreateImage(cvGetSize(image), 8 , 1); // 8byte/pixel 1 channel
    cvCopy(image, copied_image);
    cvClearMemStorage( storage );
    number_of_contours = cvFindContours(  copied_image, 
                                          storage, 
                                          &contours, 
                                          sizeof(CvContour), 
                                          CV_RETR_LIST, 
                                          CV_CHAIN_APPROX_NONE
                                        );
   printContours(contours, "contours.txt");
   removeDuplicatesInContours(contours); 
   printContours(contours, "contours_no_dup.txt");

   std::vector< std::vector<CvPoint> > image_plane_sequences; 
   getImagePlaneSequences(&contours, image_plane_sequences);
   connectNearComponents(image_plane_sequences);
   printContours(image_plane_sequences, "contours_no_dup(connected).txt"); 

   std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences;
   CvSeq* c = contours;
   int i;
   for(; c != NULL; c = (CvSeq*)(c->h_next)){
     std::vector<CvPoint2D32f> sequence; 
     for(i = 0; i < c->total; i++){
       CvPoint* pt = CV_GET_SEQ_ELEM(CvPoint, c, i);        
       CvPoint2D32f ground_pt = camera.imageToGroundPlane(*pt); 
       sequence.push_back(ground_pt);
     }
     ground_plane_sequences.push_back(sequence);
   }
   printContours(ground_plane_sequences, "contours_ground_plane.txt"); 
   scaleGroundPlaneSequences(ground_plane_sequences);
   printContours(ground_plane_sequences, "contours_ground_plane(scale).txt"); 
   drawLines(ground_plane_sequences);

}

void Contour::getImagePlaneSequences(CvSeq **contours, 
                                     std::vector< std::vector <CvPoint> >& image_plane_sequences) {
  CvSeq* c = *contours;
  int i;
  for(; c != NULL; c = (CvSeq*)(c->h_next)){
    std::vector<CvPoint> sequence; 
    for(i = 0; i < c->total; i++){
      CvPoint* pt = CV_GET_SEQ_ELEM(CvPoint, c, i);        
      sequence.push_back(cvPoint(pt->x, pt->y)); 
    }
    image_plane_sequences.push_back(sequence);
  }
  return;
}

void Contour::connectNearComponents(std::vector< std::vector<CvPoint> >& image_plane_sequences) {
  int i, j; 
  for (i = 0; i < static_cast<int>(image_plane_sequences.size()); i++) {
   CvPoint min_y = minY(image_plane_sequences[i]);
   for (j = i; j < static_cast<int>(image_plane_sequences.size()); j++) {
     CvPoint max_y = maxY(image_plane_sequences[j]); 
     if (abs(max_y.x - min_y.x) <= 2 && abs(max_y.y - min_y.y) <= 2) {
       image_plane_sequences[i].insert(image_plane_sequences[i].end(), 
                                       image_plane_sequences[j].begin(), 
                                       image_plane_sequences[j].end()); 
       image_plane_sequences.erase(image_plane_sequences.begin() + j);
     }
   }
  }
}

CvPoint Contour::minY(std::vector<CvPoint> points) {
  int min = points[0].y; 
  int i; 
  CvPoint pt;
  for (i = 0; i < static_cast<int>(points.size()); i++) {
    if (points[i].y < min) {
      min = points[i].y;
      pt = points[i];
    }
  }
  return pt;
}

CvPoint Contour::maxY(std::vector<CvPoint> points) {
  int max = points[0].y; 
  int i; 
  CvPoint pt;
  for (i = 0; i < static_cast<int>(points.size()); i++) {
    if (points[i].y > max) {
      max = points[i].y;
      pt = points[i];
    }
  }
  return pt;
}

void Contour::scaleGroundPlaneSequences(std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences){
   int i, j; 
  for (i = 0; i < static_cast<int>(ground_plane_sequences.size()); i++) {
    for (j = 0; j < static_cast<int>(ground_plane_sequences[i].size()); j++) {
      ground_plane_sequences[i][j].x = 320 + ground_plane_sequences[i][j].x * 10;
      ground_plane_sequences[i][j].y = 479 - ground_plane_sequences[i][j].y * 10;
    }
  }
}

void Contour::drawLines(std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences){

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

    //d = sqrt((double)line[0]*line[0] + (double)line[1]*line[1]); 
    //line[0] /= d; 
    //line[1] /= d; 
    //t = (float)(image_lines->width + image_lines->height); 
    //pt1.x = cvRound(line[2] - line[0]);
    //pt1.y = cvRound(line[3] - line[1]); 
    //pt2.x = cvRound(line[2] + line[0]);
    //pt2.y = cvRound(line[3] + line[1]); 

    //cvLine(image_lines, pt1, pt2, CV_RGB(0, 255, 0), 1, CV_AA, 0); 
    free(points); 
  }

  cvShowImage("fitline", image_lines); 
}

IplImage* Contour::drawContours(){
  //CvSeq* c = contours;
//  int max=0;
//  for(; c != NULL; c = (CvSeq*)(c->h_next)){
//    if(c->total > max) max = c->total;
// }

  //removeUnwantedContour(); 
  IplImage* image = cvCreateImage(cvSize(640, 480), 8 , 1);
  cvDrawContours(image, contours, cvScalar(255), cvScalar(255), 100); 

  cvNamedWindow("debug", CV_WINDOW_AUTOSIZE); 
  
  cvShowImage("debug", image);     

  cvDrawContours(copied_image, contours, cvScalar(255), cvScalar(255), 100); 
  return copied_image;
}

void Contour::removeUnwantedContour(){
  CvSeq* c = contours;
  for(; c != NULL; c = (CvSeq*)(c->h_next)){
    if(c->total < 4){       
      if(c->h_prev){
        c->h_prev->h_next = c->h_next;
      } 
      else{
        contours = c->h_next;
      }
      if(c->h_next){
        c->h_next->h_prev = c->h_prev;
      }
    } 
  }
}

void Contour::printContours(CvSeq* contours, 
                            const std::string filename){
    int i, n= 0;
    std::ofstream out(filename.c_str());
    out << "Number of contours = " << n << std::endl;
    for (CvSeq* c = contours; c!=NULL; c=c->h_next) {
      out << "Contour #" << n << std::endl;
      out << c->total << " elements:" << std::endl;
      for (i = 0; i < c->total; i++) {
        CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, c, i);        
        out << "(" << p->x << "," << p->y << ")" << std::endl; 
      }
     n++;
    }
    return;
}

void Contour::printContours(std::vector< std::vector<CvPoint2D32f> > contours, 
    const std::string filename){
  int i, j = 0; 
  std::ofstream out(filename.c_str()); 
  out << "NUmber of contours = " << contours.size() << std::endl;
  for(i = 0; i < static_cast<int>(contours.size()); i++){
    out << "Contour #" << i << std::endl; 
    out << contours[i].size() << " elements:" << std::endl;
    for(j = 0; j < static_cast<int>(contours[i].size()); j++){
      out << "(" << contours[i][j].x << ", "  << contours[i][j].y << ")" 
        << std::endl; 
    }
    //std::cout << std::endl; 
  }
  return;
}

void Contour::printContours(std::vector< std::vector<CvPoint> > contours, 
  const std::string filename){
  int i, j = 0; 
  std::ofstream out(filename.c_str()); 
  out << "NUmber of contours = " << contours.size() << std::endl;
  for(i = 0; i < static_cast<int>(contours.size()); i++){
    out << "Contour #" << i << std::endl; 
    out << contours[i].size() << " elements:" << std::endl;
    for(j = 0; j < static_cast<int>(contours[i].size()); j++){
      out << "(" << contours[i][j].x << ", "  << contours[i][j].y << ")" 
        << std::endl; 
    }
    //std::cout << std::endl; 
  }
  return;
}

void Contour::removeDuplicatesInContours(CvSeq* contours){
  int i, j;
  for(CvSeq* c = contours; c != NULL; c = c->h_next){
    for(i = 0; i < c->total; i++){
      CvPoint *current_point = CV_GET_SEQ_ELEM(CvPoint, c, i);
      for(j = i + 1; j < c->total; j++){
        //don't remove current point
        CvPoint *other_point = CV_GET_SEQ_ELEM(CvPoint, c, j);
        if( current_point->x == other_point->x && 
            current_point->y == other_point->y ){
          cvSeqRemove(c, j);
        }
      }
    }
  }
  return;
}
