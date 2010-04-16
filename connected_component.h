#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector> 
#include <iostream> 
#include <map> 
#include <cv.h>

class ConnectedComponent {

  private:
    cv::Mat _img; 
    int _label[8][8];
    std::vector<int> _parent; 
    void _union(int M, int X);
    int _find(int X);

  public:
    ConnectedComponent();
    void setImage(cv::Mat img);
    int getImgElement(int x, int y); 
    void setLabelElement(int x, int y, int value);
    int getLabelElement(int x, int y);
    std::map<std::string, int> getImgNeighbours(int x, int y);
    std::map<std::string, int> getLabelNeighbours(int x, int y);
    void runPass1();
    void runPass2();
    std::vector<int> getParent();

};

#endif

