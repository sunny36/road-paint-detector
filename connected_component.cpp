#include "connected_component.h"

ConnectedComponent::ConnectedComponent() {
}

void ConnectedComponent::setImage(cv::Mat img, 
                                  Algorithm algorithm) {
  (*this)._algorithm = algorithm;
  (*this)._img = img.clone(); 
  (*this)._label = cv::Mat::zeros((*this)._img.rows, (*this)._img.cols, CV_32S); 
  _parent.resize((*this)._img.rows * (*this)._img.cols); 
  for (int i = 0; i < static_cast<int>(_parent.size()); i++) {
    _parent[i] = 0; 
  }
}

ConnectedComponent::Algorithm ConnectedComponent::getAlgorithm() { 
  return _algorithm; 
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

std::vector<int> ConnectedComponent::getLabelNeighbours(int x, int y) {
  if((*this).getAlgorithm() == FOUR_POINT) {  
    return (*this).FourPoint(x, y); 
  } else {
    return (*this).FourPoint(x, y); 
  }
}

std::vector<int> ConnectedComponent::FourPoint(int x, int y) { 
  std::vector<int> neighbours(2); 
  neighbours[0] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; //north
  neighbours[1] = (y > 0) ? (*this).getLabelElement(x, y-1) : 0; // west
  return neighbours; 
}

std::vector<int> ConnectedComponent::NinePoint(int x, int y) { 
  std::vector<int> neighbours(10);

  neighbours[0] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  neighbours[1] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  neighbours[2] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  neighbours[3] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 

  neighbours[4] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 

  neighbours[5] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  neighbours[6] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  neighbours[7] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 
  neighbours[8] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; 

  return neighbours
}

void ConnectedComponent::runPass1() {
  int i, j; 
  int M, label; 
  std::vector<int> neighbours; 

  label = 1;
  for (i = 0; i < _img.rows; i++) {
    for (j = 0; j < _img.cols; j++) {
      if ((*this).getImgElement(i, j) != 0) {
        neighbours = (*this).getLabelNeighbours(i, j); 
        std::vector<int>::iterator it; 
        bool no_label = true; 
        //TODO change this block to for_each
        for (it = neighbours.begin(); it < neighbours.end(); ++it) { 
          if (*it != 0) { 
            //label already exists at neighbours
            no_label = false; 
            break; 
          }
        }
        if ( no_label ) {
          M = label; label++; 
        } 
        else {
          M = (*this).getMinNeighbours(neighbours);
          for (it = neighbours.begin(); it < neighbours.end(); ++it) { 
            if ((*it != M) && (*it != 0)) (*this)._union(M, *it); 
          }
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
      if((*this).getImgElement(i, j) != 0) {
        (*this).setLabelElement(i, j, 
                                (*this)._find((*this).getLabelElement(i, j))); 
      }
    }
  }
}

// Returns the minimum label which is not 0, since label starts from 1
int ConnectedComponent::getMinNeighbours(std::vector<int> neighbours) {
  std::vector<int>::iterator it; 
  int min = INT_MAX; 
  for (it = neighbours.begin(); it < neighbours.end(); ++it) { 
    if ((*it < min) && (*it != 0)) { 
      min = *it; 
    }
  }
  return min; 
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

cv::Mat ConnectedComponent::getLabel() { 
  return _label; 
}
