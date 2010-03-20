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
   printContours(contours, number_of_contours, "contours.txt");
   removeDuplicatesInContours(contours); 
   printContours(contours, number_of_contours, "contours_no_dup.txt");

   std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences;
   CvSeq* c = contours;
   int i, j;
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

   drawLines(ground_plane_sequences);
   IplImage *img1 = cvCreateImage(cvSize(640, 480), 8, 1); 
   cvNamedWindow("ground_points", CV_WINDOW_AUTOSIZE); 
  for (i = 0; i < ground_plane_sequences.size(); i++) {
    for (j = 0; j < ground_plane_sequences[i].size(); j++) {
      CvScalar s; 
      s.val[0] = 255;
      if(ground_plane_sequences[i][j].x >= 0 && 
         ground_plane_sequences[i][j].x < 640 &&
         ground_plane_sequences[i][j].y >= 0 &&
         ground_plane_sequences[i][j].y < 480) {
      cvSet2D(img1, 
              cvRound(ground_plane_sequences[i][j].x), 
              cvRound(ground_plane_sequences[i][j].y), 
              s); 

      }
    }
  }

   printContours(ground_plane_sequences, "contours_ground_plane(scale).txt"); 


   cvShowImage("ground_points", img1);
}

void Contour::scaleGroundPlaneSequences(std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences){
   int i, j; 
  for (i = 0; i < ground_plane_sequences.size(); i++) {
    for (j = 0; j < ground_plane_sequences[i].size(); j++) {
      ground_plane_sequences[i][j].x = 320 + ground_plane_sequences[i][j].x * 10;
      ground_plane_sequences[i][j].y = 479 - ground_plane_sequences[i][j].y * 10;
    }
  }
}

void Contour::drawLines(std::vector< std::vector <CvPoint2D32f> > ground_plane_sequences){

  IplImage* img = cvCreateImage(cvSize(640, 480), 8, 3);
  cvNamedWindow("fitline", CV_WINDOW_AUTOSIZE);
  cvZero(img); 
  int i, j;
  float line[4];
  CvPoint pt1, pt2; 
  float t;
  for(i = 0; i < ground_plane_sequences.size(); i++){

    CvPoint* points = (CvPoint*)malloc(ground_plane_sequences[i].size() * sizeof(points[0]));
    CvMat pointMat = cvMat(1, ground_plane_sequences[i].size(), CV_32SC2, points); 

    for(j = 0; j < ground_plane_sequences[i].size(); j++){
      points[j].x = cvRound(ground_plane_sequences[i][j].x); 
      points[j].y = cvRound(ground_plane_sequences[i][j].y); 
    }
    cvFitLine(&pointMat, CV_DIST_L1, 1, 0.001, 0.001, line); 
     fprintf(stdout, "%f %f %f %f\n", line[0], line[1], line[2], line[3]);
     pt1.x = cvRound(line[0]); 
     pt1.y = cvRound(line[1]); 
     pt2.x = cvRound(line[2]); 
     pt2.y = cvRound(line[3]); 
    cvLine(img, pt1, pt2, CV_RGB(0, 255, 0), 1, CV_AA, 0); 
    free(points); 
  }

  cvShowImage("fitline", img); 
}

IplImage* Contour::drawContours(){
  CvSeq* c = contours;
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
                            int number_of_contours, 
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
  for(i = 0; i < contours.size(); i++){
    out << "Contour #" << i << std::endl; 
    out << contours[i].size() << " elements:" << std::endl;
    for(j = 0; j < contours[i].size(); j++){
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
