#include "connected_component.h"

ConnectedComponent::ConnectedComponent() {
}

void ConnectedComponent::setImage(int (&img)[8][8]) {
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      _img[i][j] = img[i][j];
      _label[i][j] = 0; 
    }
  }

  _parent.resize(8 * 8); 
  for (i = 0; i < static_cast<int>(_parent.size()); i++) {
    _parent[i] = 0; 
  }
}

int ConnectedComponent::getImgElement(int x, int y) {
  return _img[x][y];
}

void ConnectedComponent::setLabelElement(int x, int y, int value) {
  _label[x][y] = value;
  return;
}

int ConnectedComponent::getLabelElement(int x, int y) {
  return _label[x][y];
}

boost::tuple<int, int> ConnectedComponent::getImgNeighbours(int x, int y) {
  int north, west;  
  if (x > 0) {
    north = _img[x -1][y];
  } else {
    north = 0; 
  }
  if (y > 0) {
    west = _img[x][y-1];
  } else {
    west = 0; 
  }

  return boost::make_tuple(north, west);
}

boost::tuple<int, int> ConnectedComponent::getLabelNeighbours(int x, int y) {
  int north, west; 

  if (x > 0) {
    north = _label[x -1][y];
  } else {
    north = 0; 
  }
  if (y > 0) {
    west = _label[x][y-1];
  } else {
    west = 0; 
  }

  return boost::make_tuple(north, west);
}

void ConnectedComponent::runPass1() {
  int i, j, north, west; 
  int M, label; 

  label = 1;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (_img[i][j] == 1) {
        boost::tie(north, west) = (*this).getLabelNeighbours(i, j); 
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
      if(_img[i][j] == 1) {
        _label[i][j] = (*this)._find(_label[i][j]); 
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

  //std::cout << X << ", " << Y << std::endl;
}

int ConnectedComponent::_find(int X) {
  int j = X; 
  while (_parent[j] != 0) { 
    j = _parent[j]; 
  }

  return j;
}

std::vector<int> ConnectedComponent::getParent() { 
  return _parent; 
}
