#include "../connected_component.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <map>
#include <cv.h>


class ConnectedComponentTest : public testing::Test { 

};

class MockDataTest : public ConnectedComponentTest { 
  protected:
    virtual void SetUp() { 
      int data[8][8] = { 
        {1, 1, 0, 1, 1, 1, 0, 1},
        {1, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1},
        {0, 0, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 0, 1, 1, 1}
      };
       IplImage* img = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);
       int i, j;
       for (i = 0; i < 8; ++i) { 
         for (j = 0; j < 8; ++j) { 
          CvScalar scalar; 
          scalar.val[0] = data[i][j]; 
          cvSet2D(img, i, j, scalar); 
         }
       }

       ConnectedComponent::Algorithm alg = ConnectedComponent::FOUR_POINT;
       connected_component.setImage(img, alg);
    }
    ConnectedComponent connected_component;
};

class ImageDataTest : public ConnectedComponentTest { 
  protected: 
    virtual void SetUp() { 
      IplImage* img = cvLoadImage("../localmaxima.png", CV_LOAD_IMAGE_GRAYSCALE); // 0 for grayscale 
      ConnectedComponent::Algorithm alg = ConnectedComponent::NINE_POINT;
      connected_component.setImage(img, alg); 
      
/*      for (i = 0; i < img2->width; ++i) { */
        //CvScalar scalar = cvGet2D(img2, 350, i); 
        //std::cout << scalar.val[0] << " "; 
        //if(scalar.val[0] == 255) { 
          ////std::cout << i << std::endl;
        //}
/*      }*/
    }
    ConnectedComponent connected_component; 
};

TEST_F(MockDataTest, ShouldReturnCorrectValue) {
  EXPECT_EQ(1, connected_component.getImgElement(0, 0)); 
  EXPECT_EQ(0, connected_component.getImgElement(6, 6)); 
  EXPECT_EQ(1, connected_component.getImgElement(7, 7)); 
}

TEST_F(MockDataTest, ShouldSetLabelValueGivenXY) {
  connected_component.setLabelElement(0, 0, 1); 
  EXPECT_EQ(1, connected_component.getLabelElement(0,0)); 
  
  connected_component.setLabelElement(3, 3, 1); 
  EXPECT_EQ(1, connected_component.getLabelElement(3,3)); 
  
  EXPECT_EQ(0, connected_component.getLabelElement(7,7)); 
}

TEST_F(MockDataTest, ShouldReturnCorrectNeighbours) {
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

TEST_F(MockDataTest, ShouldReturnCorrectResultAfterPass1) { 
  connected_component.runPass1(); 
 
/*  CvMat* label = connected_component.getLabel(); */
  //std::cout << label->height << std::endl; 
  //std::cout << label->width << std::endl;
  //int i, j;
  //for (i = 0; i < label->height; i++) {
    //for (j = 0; j < label->width; j++) {
      //CvScalar scalar; 
      //scalar = cvGet2D(label, i, j); 
      //std::cout <<  scalar.val[0] << " "; 
    //}
    //std::cout << "\n"; 
/*  }*/
   
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

TEST_F(MockDataTest, ShouldReturnCorrectLabelAfterPass2) { 
  /* must run pass1 before pass2 */
  connected_component.runPass1(); 
  connected_component.runPass2(); 
/*  CvMat* label = connected_component.getLabel(); */
  //std::cout << label->height << std::endl; 
  //std::cout << label->width << std::endl;
  //int i, j;
  //for (i = 0; i < label->height; i++) {
    //for (j = 0; j < label->width; j++) {
      //CvScalar scalar; 
      //scalar = cvGet2D(label, i, j); 
      //std::cout <<  scalar.val[0] << " "; 
    //}
    //std::cout << "\n"; 
/*  }*/ 
  EXPECT_EQ(1, connected_component.getLabelElement(0, 4)); 
  EXPECT_EQ(1, connected_component.getLabelElement(1, 5)); 
  EXPECT_EQ(3, connected_component.getLabelElement(7, 5)); 
  EXPECT_EQ(3, connected_component.getLabelElement(7, 6)); 
}

TEST_F(ImageDataTest, ShouldReturnConnectedComponent) { 
  connected_component.runPass1(); 
  connected_component.runPass2(); 

  CvMat* label = connected_component.getLabel(); 
  IplImage* img = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
  CvScalar label_value, image_value;
  for (int i = 0; i < label->height; ++i) {
    for (int j = 0; j < label->width; ++j) { 
      label_value = cvGet2D(label, i, j); 
      if (label_value.val[0] != 0) {
        image_value.val[0] = 255;
        cvSet2D(img, i, j, image_value); 
      }
    }
  }
  cvSaveImage("output.png", img); 
  label_value = cvGet2D(label, 350, 406); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 351, 407); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 352, 409); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 353, 412); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 354, 414); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 355, 416); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 356, 418); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 357, 420); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 358, 423); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 359, 425); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 360, 427); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 361, 429); 
  std::cout << label_value.val[0] <<  std::endl; 
  label_value = cvGet2D(label, 362, 429); 
  std::cout << label_value.val[0] <<  std::endl; 
}

TEST_F(ImageDataTest, TestGetConnectedComponent) { 
  connected_component.runPass1(); 
  connected_component.runPass2(); 


  std::vector<std::vector<CvPoint> > sequences; 
  std::vector<std::vector<CvPoint> >::iterator it; 
  sequences =   connected_component.get_connected_component(); 
  int i = 0; 
  for (it = sequences.begin(); it != sequences.end(); ++it) { 
    std::cout << "Number #" << i++ << std::endl; 
    std::vector<CvPoint> points; 
    std::vector<CvPoint>::iterator it_points; 
    points = *it; 
    for (it_points = points.begin(); it_points != points.end(); ++it_points) { 
      std::cout << "(" << (*it_points).x << "," << (*it_points).y << ")" 
                << std::endl; 
    }
    std::cout << std::endl; 
  }
}

