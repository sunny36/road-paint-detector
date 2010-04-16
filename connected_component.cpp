#include "connected_component.h"

ConnectedComponent::ConnectedComponent() {
}

void ConnectedComponent::setImage(cv::Mat img) {
  (*this)._img = img.clone(); 
  (*this)._label = cv::Mat::zeros((*this)._img.rows, (*this)._img.cols, CV_32S); 
  int i;
  _parent.resize(8 * 8); 
  for (i = 0; i < static_cast<int>(_parent.size()); i++) {
    _parent[i] = 0; 
  }
}

int ConnectedComponent::getImgElement(int x, int y) {
  return _img.at<int>(x, y);
}

void ConnectedComponent::setLabelElement(int x, int y, int value) {
  (*this)._label.at<int>(x, y) = value;
  return;
}

int ConnectedComponent::getLabelElement(int x, int y) {
  return (*this)._label.at<int>(x, y);
}

std::vector<int> ConnectedComponent::getParent() { 
  return _parent; 
}

std::map<std::string, int> ConnectedComponent::getImgNeighbours(int x, int y) {
  int north, west; 
  north = (x > 0) ? (*this).getImgElement(x - 1, y) : 0; 
  west = (y > 0) ? (*this).getImgElement(x, y - 1) : 0; 

  std::map<std::string, int> neighbours; 
  neighbours["north"] = north; 
  neighbours["west"] = west; 
 
  return neighbours;
}

std::map<std::string, int> ConnectedComponent::getLabelNeighbours(int x, int y) {
  int north, west; 
  north = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  west = (y > 0) ? (*this).getLabelElement(x, y-1) : 0; 

  std::map<std::string, int> neighbours; 
  neighbours["north"] = north; 
  neighbours["west"] = west; 
 
  return neighbours;
}


void ConnectedComponent::runPass1() {
  int i, j, north, west; 
  int M, label; 
  std::map<std::string, int> neighbours; 

  label = 1;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if ((*this).getImgElement(i, j) == 1) {
        neighbours = (*this).getLabelNeighbours(i, j); 
        north = neighbours["north"]; 
        west = neighbours["west"]; 
        if ( (north==0) && (west==0) ) {
          M = label; label++; 
        } 
        else {
          if(north == 0) {
            M = west;
          }
          else if(west == 0) {
            M = north;
          }
          else {
            M = (north < west) ? north : west; 
          }
          if ((north != M) && (north != 0)) (*this)._union(M, north);
          if (west != M && (west != 0)) (*this)._union(M, west);
        }
        (*this).setLabelElement(i, j, M);
      }
    }
  }
}

void ConnectedComponent::runPass2() { 
  int i, j; 
  for(i = 0; i < 8; i++) { 
    for(j = 0; j < 8; j++) { 
      if((*this).getImgElement(i, j) == 1) {
        (*this).setLabelElement(i, j, (*this)._find((*this).getLabelElement(i, j))); 
      }
    }
  }
}

void ConnectedComponent::_union(int X, int Y) {
  int j = X; 
  int k = Y; 
  while(_parent[j] != 0) {
    j = _parent[j]; 
  }
  while(_parent[k] != 0) { 
    k = _parent[k]; 
  }
  if (j != k ) {
    _parent[k] = j; 
  }

}

int ConnectedComponent::_find(int X) {
  int j = X; 
  while (_parent[j] != 0) { 
    j = _parent[j]; 
  }

  return j;
}
