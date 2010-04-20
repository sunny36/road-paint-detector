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
    enum Algorithm {FOUR_POINT, TWENTY_ONE_POINT};
    void setImage(cv::Mat img, ConnectedComponent::Algorithm algorithm);
    int getImgElement(int x, int y); 
    void setLabelElement(int x, int y, int value);
    int getLabelElement(int x, int y);
    cv::Mat getLabel(); 
    std::map<std::string, int> getImgNeighbours(int x, int y);
    std::map<std::string, int> getLabelNeighbours(int x, int y);
    void runPass1();
    void runPass2();
    std::vector<int> getParent();
    ConnectedComponent::Algorithm getAlgorithm();

  private:
    cv::Mat _img; 
    cv::Mat _label; 
    std::vector<int> _parent; 
    ConnectedComponent::Algorithm _algorithm; 
    void _union(int M, int X);
    int _find(int X);
    int getMinNeighbours(std::map<std::string, int> neighbours);

};

#endif

