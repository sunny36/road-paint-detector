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


