/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: USER
 *
 * Created on 31 de agosto de 2016, 11:56 PM
 */

#include <QApplication>
#include "MainForm.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "preprocessing.h"
#include "feature_extraction.h"
#include <cstdlib>
using namespace std;
using namespace cv;
int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    MainForm mainForm;
    mainForm.show();
    Mat image;
    
    image = imread("image/0-dataset/image1.tif", 1);
    
    mean(image);
    if (!image.data) {
        printf(" No image data \n ");
        return -1;
    }
    Mat bgMask, odImage, maImage;
    
    //Preprocessing
    backgroundSegmentation(image, bgMask);
    opticDiscSegmentation(bgMask, maImage);
    darkLessionSegmentation(bgMask, maImage);//microaneurysm
    
    //vesselSegmentation(bgMask,maImage);
    
    //brightLessionSegmentation();
    //noiseSegmentation(image,noiseMask);
    //finalSegmentation(image,backGroundMask,noiseMask,finalMask,result);
    
    //Vessel segmentation
    //imageVesselEnhancement(result,finalMask);
    
    
    waitKey(0);
       
    // create and show your widgets here

    return app.exec();
}
