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

   printContours(ground_plane_sequences, "contours_ground_plane(scale).txt"); 

}

void Contour::scaleGroundPlaneSequences(
            std::vector< std::vector <CvPoint2D32f> >& ground_plane_sequences){
   int i, j; 
  for (i = 0; i < ground_plane_sequences.size(); i++) {
    for (j = 0; j < ground_plane_sequences[i].size(); j++) {
      ground_plane_sequences[i][j].x = 320 + ground_plane_sequences[i][j].x * 10;
      ground_plane_sequences[i][j].y = 479 - ground_plane_sequences[i][j].y * 10;
    }
  }
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
