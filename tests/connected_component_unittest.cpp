#include "../connected_component.h"
#include <gtest/gtest.h>
#include <cstdio>


class ConnectedComponentTest : public testing::Test { 
  protected:
    virtual void SetUp() { 
      int img[8][8] = { 
        {1, 1, 0, 1, 1, 1, 0, 1},
        {1, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1},
        {0, 0, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 0, 1, 1, 1}
      };

      connected_component.setImage(img);
    }

    ConnectedComponent connected_component;
};

TEST_F(ConnectedComponentTest, ShouldReturnCorrectValue) {
  EXPECT_EQ(connected_component.getImgElement(0, 0), 1); 
  EXPECT_EQ(connected_component.getImgElement(6, 6), 0); 
  EXPECT_EQ(connected_component.getImgElement(7, 7), 1); 
}

TEST_F(ConnectedComponentTest, ShouldSetLabelValueGivenXY) {
  connected_component.setLabelElement(0, 0, 1); 
  EXPECT_EQ(1, connected_component.getLabelElement(0,0)); 
  
  connected_component.setLabelElement(3, 3, 1); 
  EXPECT_EQ(1, connected_component.getLabelElement(3,3)); 
  
  EXPECT_EQ(0, connected_component.getLabelElement(7,7)); 


}

TEST_F(ConnectedComponentTest, ShouldReturnCorrectNeighbours) {
  int north, west; 
  boost::tie(north, west) = connected_component.getImgNeighbours(0, 0);
  EXPECT_EQ(north, 0); 
  EXPECT_EQ(west, 0);

  boost::tie(north, west) = connected_component.getImgNeighbours(1, 3);
  EXPECT_EQ(north, 1); 
  EXPECT_EQ(west, 0);

  boost::tie(north, west) = connected_component.getImgNeighbours(0, 6);
  EXPECT_EQ(north, 0); 
  EXPECT_EQ(west, 1);

  boost::tie(north, west) = connected_component.getImgNeighbours(4, 4);
  EXPECT_EQ(north, 0); 
  EXPECT_EQ(west, 1);

  boost::tie(north, west) = connected_component.getImgNeighbours(7, 7);
  EXPECT_EQ(north, 1); 
  EXPECT_EQ(west, 1);
}

TEST_F(ConnectedComponentTest, ShouldReturnCorrectResultAfterPass1) { 
  int i, j, north, west; 
  int M, label; 

  label = 1;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (connected_component.getImgElement(i, j) == 1) {
        boost::tie(north, west) = connected_component.getLabelNeighbours(i, j); 
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
        }
        connected_component.setLabelElement(i, j, M);
      }
    }
  }

  EXPECT_EQ(1, connected_component.getLabelElement(0, 0));
  EXPECT_EQ(2, connected_component.getLabelElement(0, 4));
  EXPECT_EQ(3, connected_component.getLabelElement(0, 7));
  EXPECT_EQ(4, connected_component.getLabelElement(4, 3));
  EXPECT_EQ(5, connected_component.getLabelElement(4, 5));
  EXPECT_EQ(6, connected_component.getLabelElement(7, 0));

}



