/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: espri
 *
 * Created on 1 de diciembre de 2015, 11:37 PM
 */

#include <cstdlib>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "util.h"
using namespace std;
using namespace cv;

/*
 * 
 */
int main(int argc, char** argv) {
    Mat image;
    
    image = imread("img/eye_test.tif", 1);
    
    mean(image);
    if (!image.data) {
        printf(" No image data \n ");
        return -1;
    }
    Mat backGroundMask;
    //Mat hsiImage;
    
    //bgrToHsi(image,hsiImage);
    backgroundSegmentation(image, backGroundMask);
    
    //mwrite("img/hsi.tif",hsiImage);
    
    /*namedWindow("Image", CV_WINDOW_AUTOSIZE);
    imshow("Image", hsiImage);*/
    waitKey(0);
    return 0;
}

