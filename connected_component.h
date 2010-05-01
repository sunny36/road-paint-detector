#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector> 
#include <iostream> 
#include <map> 
#include <cv.h>
#include <highgui.h> 

class ConnectedComponent {


  public:
    ConnectedComponent();
    enum Algorithm {FOUR_POINT, NINE_POINT};
    void setImage(IplImage* img, ConnectedComponent::Algorithm algorithm);
    int getImgElement(int x, int y); 
    void setLabelElement(int x, int y, int value);
    int getLabelElement(int x, int y);
    CvMat* getLabel(); 
    cv::Mat* getImg(); 
    std::map<std::string, int> getImgNeighbours(int x, int y);
    std::vector<int> getLabelNeighbours(int x, int y);
    void runPass1();
    void runPass2();
    std::vector<int> getParent();
    ConnectedComponent::Algorithm getAlgorithm();
    std::map<int, std::vector<CvPoint> > get_connected_component();

  private:
    IplImage* _img; 
    CvMat* _label; 
    std::vector<int> _parent; 
    Algorithm _algorithm; 
    void _union(int M, int X);
    int _find(int X);
    int getMinNeighbours(std::vector<int> neighbours);
    std::vector<int> FourPoint(int x, int y);
    std::vector<int> NinePoint(int x, int y); 
};

#endif

