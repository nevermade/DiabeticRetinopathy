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
#include<fstream>
#include<iostream>
#include<string>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace cv;

void classifyBD();

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    /*QApplication app(argc, argv);
    MainForm mainForm;
    mainForm.show();*/
    Mat image;

    image = imread("image/0-dataset/image1.tif", 1);

    mean(image);
    if (!image.data) {
        printf(" No image data \n ");
        return -1;
    }
    Mat bgMask, odImage, maImage;

    //clasificar bd
    classifyBD();



    //Preprocessing
    backgroundSegmentation(image, bgMask);
    opticDiscSegmentation(bgMask, maImage);
    darkLessionSegmentation(bgMask, maImage); //microaneurysm

    //vesselSegmentation(bgMask,maImage);

    //brightLessionSegmentation();
    //noiseSegmentation(image,noiseMask);
    //finalSegmentation(image,backGroundMask,noiseMask,finalMask,result);

    //Vessel segmentation
    //imageVesselEnhancement(result,finalMask);


    waitKey(0);

    // create and show your widgets here

    //return app.exec();
    return 0;
}

void classifyBD() {
    string baseURL = "image/0-dataset/";

    string name;
    int type, counter_n = 1, counter_l = 1, counter_m = 1, counter_s = 1;
    Mat image;
    for (int i = 1; i <= 4; i++) {
        ifstream myReadFile(baseURL + "dataset "+SSTR(i)+".txt");
        if (myReadFile.is_open()) {
            while (myReadFile >> name >> type) {
                image = imread(baseURL + "dataset "+SSTR(i)+"/" + name, 1);
                switch (type) {
                    case 0:
                        imwrite(baseURL + "0 - normal/" + "n_image" + SSTR(counter_n) + ".tif", image);
                        counter_n++;
                        break;
                    case 1:
                        imwrite(baseURL + "1 - leve/" + "l_image" + SSTR(counter_l) + ".tif", image);
                        counter_l++;
                        break;
                    case 2:
                        imwrite(baseURL + "2 - moderado/" + "m_image" + SSTR(counter_m) + ".tif", image);
                        counter_m++;
                        break;
                    case 3:
                        imwrite(baseURL + "3 - severo/" + "s_image" + SSTR(counter_s) + ".tif", image);
                        counter_s++;
                        break;
                }


            }
        } else {
            cerr << "Error: " << strerror(errno);
        }
        myReadFile.close();
    }

    
}