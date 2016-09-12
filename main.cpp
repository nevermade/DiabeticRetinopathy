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
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "preprocessing.h"
#include "feature_extraction.h"
#include <cstdlib>
#include<fstream>
#include<iostream>
#include<string>



using namespace std;
using namespace cv;

void classifyBD();

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    /*QApplication app(argc, argv);
    MainForm mainForm;
    mainForm.show();*/
    
    //clasificar bd
    //classifyBD();
    int numberOfImages = 20;
    int nCategories = 4;
    vector<CharsImage> chars;
    string urlBase = "";
    string image_name="";
    Mat bgMask,image,tmp;
    //Preprocessing
    int flag=1;
    for (int i = 0; i < nCategories; i++) {
        switch (i) {
            case 0:
                urlBase = "image/0-dataset/0 - normal/";
                image_name="n_image";
                break;
            case 1:
                urlBase = "image/0-dataset/0 - leve/";
                image_name="l_image";
                break;
            case 2:
                urlBase = "image/0-dataset/0 - moderado/";
                image_name="m_image";
                break;
            case 3:
                urlBase = "image/0-dataset/0 - severo/";
                image_name="s_image";
                break;

        }
        for (int n = 1; n <= numberOfImages; n++) {
            CharsImage ci;
            ci.nImage=n;
            
            image = imread(urlBase + image_name + SSTR(n)+ ".tif", 1);
            if(flag){
                backgroundSegmentation(image, bgMask);
                flag=0;
            }
            bgMask.copyTo(tmp);
            opticDiscSegmentation(tmp, image);
            //imwrite("image/finalMaskImage/image"+SSTR(ci.nImage)+".tif", image);
            darkLessionSegmentation(tmp, image, ci);            
            brightLessionSegmentation(tmp, image, ci);
            
            //chars.push_back(ci);
            //cout<<ci.areaDarkZone<<" "<<ci.numberDarkZone<<" "<<ci.areaBrightZone<<" "<<ci.numberBrightZones<<endl;
            cout<<ci.areaDarkZone<<" "<<ci.areaBrightZone<<endl;
            
        }
        break;
    }
    



    //vesselSegmentation(bgMask,maImage);



    waitKey(0);
    //return app.exec();
    return 0;
}

void classifyBD() {
    string baseURL = "image/0-dataset/";

    string name;
    int type, counter_n = 1, counter_l = 1, counter_m = 1, counter_s = 1;
    Mat image;
    for (int i = 1; i <= 4; i++) {
        ifstream myReadFile(baseURL + "dataset " + SSTR(i) + ".txt");
        if (myReadFile.is_open()) {
            while (myReadFile >> name >> type) {
                image = imread(baseURL + "dataset " + SSTR(i) + "/" + name, 1);
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