#include "connected_component.h"

ConnectedComponent::ConnectedComponent() {
}

void ConnectedComponent::setImage(IplImage* img, 
                                  Algorithm algorithm) {
  (*this)._algorithm = algorithm;
  (*this)._img = cvCloneImage(img); 

  (*this)._label = cvCreateMat((*this)._img->height, (*this)._img->width, CV_16UC1); 
  cvSetZero((*this)._label);
  _parent.resize((*this)._img->height * (*this)._img->width); 
  for (int i = 0; i < static_cast<int>(_parent.size()); i++) {
    _parent[i] = 0; 
  }
}

ConnectedComponent::Algorithm ConnectedComponent::getAlgorithm() { 
  return _algorithm; 
}

int ConnectedComponent::getImgElement(int x, int y) {
  CvScalar scalar; 
  scalar = cvGet2D(_img, x, y);
  return scalar.val[0]; 
}

void ConnectedComponent::setLabelElement(int x, int y, int value) {
  CvScalar scalar; 
  scalar.val[0] = value; 
  cvSet2D((*this)._label, x, y, scalar); 
  return; 
}

int ConnectedComponent::getLabelElement(int x, int y) {
  CvScalar scalar; 
  scalar = cvGet2D(_label, x, y); 
  return scalar.val[0]; 
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
  if((*this)._algorithm == FOUR_POINT) { 
    return (*this).FourPoint(x, y); 
  }
  else { 
    return (*this).NinePoint(x, y); 
  }
}

std::vector<int> ConnectedComponent::FourPoint(int x, int y) { 
  std::vector<int> neighbours(2); 
  neighbours[0] = (x > 0) ? (*this).getLabelElement(x-1, y) : 0; //north
  neighbours[1] = (y > 0) ? (*this).getLabelElement(x, y-1) : 0; // west
  return neighbours; 
}

std::vector<int> ConnectedComponent::NinePoint(int x, int y) { 
  std::vector<int> neighbours(9);
  std::vector<int>::iterator it; 
  int iNeighbours = -4; //extreme left is y - 4

  neighbours[0] = ((x > 0) && (y > 3)) ? 
                  (*this).getLabelElement(x - 1, y - 4) : 0; 
  neighbours[1] = ((x > 0) && (y > 2)) ? 
                  (*this).getLabelElement(x - 1, y - 3) : 0; 
  neighbours[2] = ((x > 0) && (y > 1)) ? 
                  (*this).getLabelElement(x - 1, y - 2) : 0; 
  neighbours[3] = ((x > 0) && (y > 0)) ? 
                  (*this).getLabelElement(x - 1, y - 1) : 0; 

  neighbours[4] = (x > 0)  ? (*this).getLabelElement(x - 1, y) : 0; 

  neighbours[5] = ((x > 0) && (y < 639)) ? 
                  (*this).getLabelElement(x - 1, y + 1) : 0; 
  neighbours[6] = ((x > 0) && (y < 638)) ? 
                  (*this).getLabelElement(x - 1, y + 2) : 0; 
  neighbours[7] = ((x > 0) && (y < 637)) ? 
                  (*this).getLabelElement(x - 1, y + 3) : 0; 
  neighbours[8] = ((x > 0) && (y < 636)) ? 
                  (*this).getLabelElement(x - 1, y + 4) : 0; 

  return neighbours;
}

void ConnectedComponent::runPass1() {
  int i, j; 
  int M, label; 
  std::vector<int> neighbours; 

  label = 1;
  for (i = 0; i < _img->height; i++) {
    for (j = 0; j < _img->width; j++) {
      if ((*this).getImgElement(i, j) != 0 ) {
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
  for(i = 0; i < _img->height; i++) { 
    for(j = 0; j < _img->width; j++) { 
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

CvMat* ConnectedComponent::getLabel() { 
  return _label; 
}


