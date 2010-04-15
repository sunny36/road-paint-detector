#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <boost/tuple/tuple.hpp> 
#include <vector> 

class ConnectedComponent {

  private:
    int _img[8][8]; 
    int _label[8][8];
    std::vector<int> _parent; 

  public:
    ConnectedComponent();
    void setImage(int (&img)[8][8]);
    int getImgElement(int x, int y); 
    void setLabelElement(int x, int y, int value);
    int getLabelElement(int x, int y);
    boost::tuple<int, int> getImgNeighbours(int x, int y);
    boost::tuple<int, int> getLabelNeighbours(int x, int y);
};

#endif

