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
  std::map<std::string, int> neighbours;

  neighbours = connected_component.getImgNeighbours(0, 0);
  EXPECT_EQ(0, neighbours["north"]); 
  EXPECT_EQ(0, neighbours["west"]);

  neighbours = connected_component.getImgNeighbours(1, 3);
  EXPECT_EQ(1, neighbours["north"]); 
  EXPECT_EQ(0, neighbours["west"]);

  neighbours = connected_component.getImgNeighbours(0, 6);
  EXPECT_EQ(0, neighbours["north"]); 
  EXPECT_EQ(1, neighbours["west"]);

  neighbours = connected_component.getImgNeighbours(4, 4);
  EXPECT_EQ(0, neighbours["north"]); 
  EXPECT_EQ(1, neighbours["west"]);

  neighbours = connected_component.getImgNeighbours(7, 7);
  EXPECT_EQ(1, neighbours["north"]); 
  EXPECT_EQ(1, neighbours["west"]);
}

TEST_F(ConnectedComponentTest, ShouldReturnCorrectResultAfterPass1) { 
  connected_component.runPass1(); 
  
  EXPECT_EQ(1, connected_component.getLabelElement(0, 0));
  EXPECT_EQ(2, connected_component.getLabelElement(0, 4));
  EXPECT_EQ(3, connected_component.getLabelElement(0, 7));
  EXPECT_EQ(4, connected_component.getLabelElement(4, 3));
  EXPECT_EQ(5, connected_component.getLabelElement(4, 5));
  EXPECT_EQ(6, connected_component.getLabelElement(7, 0));

  std::vector<int> parent = connected_component.getParent(); 

  EXPECT_EQ(1, parent[2]); 
  EXPECT_EQ(3, parent[7]); 

  connected_component.runPass2(); 
}

TEST_F(ConnectedComponentTest, ShouldReturnCorrectLabelAfterPass2) { 
  //must run pass1 before pass2
  connected_component.runPass1(); 
  connected_component.runPass2(); 

  EXPECT_EQ(1, connected_component.getLabelElement(0, 4)); 
  EXPECT_EQ(1, connected_component.getLabelElement(1, 5)); 
  EXPECT_EQ(3, connected_component.getLabelElement(7, 5)); 
  EXPECT_EQ(3, connected_component.getLabelElement(7, 6)); 
}


