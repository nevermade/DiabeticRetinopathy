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
using namespace cv::ml;

void classifyBD();
bool file_exists(const string& name);

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    /*QApplication app(argc, argv);
    MainForm mainForm;
    mainForm.show();*/

    //clasificar bd
    //classifyBD();
    int numberOfImages = 25;
    int nChars = 3;
    int nCategories = 4;
    string urlBase = "";
    string image_name = "";
    Mat bgMask, image, tmp;
    int labels[numberOfImages * nCategories];
    float trainingData[numberOfImages * nCategories][nChars];
    //Preprocessing
    int flag = 1;
    int nImage = 0;
    string svmFileName = "svm_training.xml";
    string trainingFileName = "training_data.csv";
    ifstream svmFile(svmFileName);
    ifstream trainingFile(trainingFileName);    
    Ptr<TrainData> td;
    if (!svmFile.is_open()) {
        if (!trainingFile.is_open()) {
            ofstream trainingFileOutput(trainingFileName);
            for (int i = 0; i < nCategories; i++) {
                switch (i) {
                    case 0:
                        urlBase = "image/0-dataset/0 - normal/";
                        image_name = "n_image";
                        break;
                    case 1:
                        urlBase = "image/0-dataset/1 - leve/";
                        image_name = "l_image";
                        break;
                    case 2:
                        urlBase = "image/0-dataset/2 - moderado/";
                        image_name = "m_image";
                        break;
                    case 3:
                        urlBase = "image/0-dataset/3 - severo/";
                        image_name = "s_image";
                        break;

                }
                for (int n = 1; n <= numberOfImages; n++) {
                    CharsImage ci;
                    ci.nImage = n;

                    image = imread(urlBase + image_name + SSTR(n) + ".tif", 1);
                    if (flag) {
                        backgroundSegmentation(image, bgMask);
                        flag = 0;
                    }
                    bgMask.copyTo(tmp);
                    opticDiscSegmentation(tmp, image, ci);
                    //cout<<ci.nImage+i*numberOfImages<<endl;
                    //imwrite("image/3-final mask/image"+SSTR(ci.nImage)+".tif", tmp);
                    darkLessionSegmentation(tmp, image, ci);
                    brightLessionSegmentation(tmp, image, ci);


                    //cout<<"Imagen "<<nImage<<": "<<"(Tipo - "<<i<<") "<<ci.areaDarkZone<<" "<<ci.numberDarkZone<<" "<<ci.areaBrightZone<<" "<<ci.numberBrightZones<<endl;
                    cout << "Imagen " << nImage << ": " << "(Tipo - " << i << ") " << ci.areaDarkZone << " " << ci.numberDarkZone << " " << ci.areaBrightZone << endl;
                    trainingFileOutput << ci.areaDarkZone << "," << ci.numberDarkZone << "," << ci.areaBrightZone << ","<<i<<endl;
                    //cout<<ci.areaDarkZone<<" "<<ci.areaBrightZone<<endl;
                    labels[nImage] = i;
                    trainingData[nImage][0] = ci.areaDarkZone;
                    trainingData[nImage][1] = ci.numberDarkZone;
                    trainingData[nImage][2] = ci.areaBrightZone;
                    //trainingData[nImage][4] = ci.numberBrightZones;
                    nImage++;
                }               
            }
            trainingFileOutput.close();
            Mat labelsMat(numberOfImages*nCategories, 1, CV_32SC1, labels);
            Mat trainingDataMat(numberOfImages*nCategories, nChars, CV_32FC1, trainingData);
            td = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
        } else {
            td = TrainData::loadFromCSV(trainingFileName, -1);
            svmFile.close();
        }

        Ptr<SVM> svm = SVM::create();
        /*svm->setType(ml::SVM::C_SVC);
        svm->setKernel(ml::SVM::    POLY);
        svm->setGamma(3);
        svm->setDegree(3);*/
        svm->trainAuto(td, 5);
        svm->save(svmFileName);
        

    } else {
        cout << "File Exists" << endl;
        Ptr<SVM> svm = SVM::create();
        svm = SVM::load<SVM>(svmFileName);
        cout << "Config SVM file opened..." << endl;
        float test[1][3] = {
            {95, 3, 1705}
        };
        Mat sampleMat(1, nChars, CV_32FC1, test);
        //cout << svm->getVarCount() << endl;
        float result = svm->predict(sampleMat);
        cout << "El resultado es: " << result << endl;
    }


    svmFile.close();


    //svm->load("svm_result.xml");
    //vesselSegmentation(bgMask,maImage);



    waitKey(0);
    //return app.exec();
    return 0;
}

bool file_exists(const string& name) {
    ifstream ifile(name);
    return ifile.good();

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