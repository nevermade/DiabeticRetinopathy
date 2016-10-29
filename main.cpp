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
#include<dirent.h>


using namespace std;
using namespace cv;
using namespace cv::ml;

void classifyBD();
bool file_exists(const string& name);
void generateTestFile();
void printMatrix(int matrix[][4]);
void getCharsFromTestFolder(string& svmFileName);

int numberOfImages = 150;
int nChars = 4;
int nCategories = 4;

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    MainForm mainForm;
    mainForm.show();

    //clasificar bd
    //classifyBD();
    //return 0;
    //generate testset file
    //generateTestFile();
    //return 0;

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
    ofstream trainingFileOutput;
    Ptr<TrainData> td;
    CharsImage ci;
    int ncols = 2240, nrows = 1488;
    int debugFlag = 0;
    if (!svmFile.is_open() || debugFlag) {
        if (!trainingFile.is_open() || debugFlag) {
            if (!debugFlag) trainingFileOutput.open(trainingFileName);
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
                    ci.nImage = n;

                    image = imread(urlBase + image_name + SSTR(n) + ".tif", 1);
                    if (image.cols != ncols || image.rows != nrows) {
                        resize(image, image, Size(ncols, nrows));
                    }
                    if (flag) {
                        //backgroundSegmentation(image, bgMask);
                        readInGreenChannel("image/bgMask.tif",bgMask);
                        flag = 0;
                    }
                    bgMask.copyTo(tmp);
                    opticDiscSegmentation(tmp, image, ci);
                    //cout<<ci.nImage+i*numberOfImages<<endl;
                    //imwrite("image/3-final mask/image"+SSTR(ci.nImage)+".tif", tmp);
                    darkLessionSegmentation(tmp, image, ci);
                    brightLessionSegmentation(tmp, image, ci);



                    cout << "Imagen " << nImage << ": " << "(Tipo - " << i << ") " << ci.areaDarkZone << " " << ci.numberDarkZone << " " << ci.areaBrightZone << " " << ci.numberBrightZones << endl;
                    //cout << "Imagen " << nImage << ": " << "(Tipo - " << i << ") " << ci.areaDarkZone << " " << ci.numberDarkZone <<" " << ci.areaBrightZone <<endl;
                    trainingFileOutput << image_name <<","<< ci.areaDarkZone << "," << ci.numberDarkZone << "," << ci.areaBrightZone << "," << ci.numberBrightZones << "," << i << endl;
                    //cout<<ci.areaDarkZone<<" "<<ci.areaBrightZone<<endl;
                    labels[nImage] = i;
                    trainingData[nImage][0] = ci.areaDarkZone;
                    trainingData[nImage][1] = ci.numberDarkZone;
                    trainingData[nImage][2] = ci.areaBrightZone;
                    trainingData[nImage][3] = ci.numberBrightZones; 
                    nImage++;
                    if (debugFlag) {
                        return 0;
                    }

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
        svm->setKernel(ml::SVM::POLY);
        svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6));
        svm->setDegree(3);*/
        svm->setType(ml::SVM::C_SVC);
        svm->setKernel(ml::SVM::RBF);
        svm->setGamma(9.3750000000000002e-002);
        svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1.1920928955078125e-007));
        svm->setC(2.5000000000000000e+000);
        ParamGrid noParams;
        noParams.logStep = 0;

        svm->trainAuto(td, 10, noParams,
                noParams,
                noParams,
                noParams,
                noParams,
                noParams,
                true);
        ;
        svm->save(svmFileName);


    } else {
        cout << "File Exists" << endl;

        //getCharsFromTestFolder(svmFileName);

        /*int nrows = testChars.rows;
        //float* ptr;
       
        svm->predict(testChars,results);
        
        for (int i = 0; i < nrows; i++) {
            cout << "Image " << i << ": " <<results.at<float>(i,0)<< endl;
        }*/
        /*float result;
        Mat sampleMat;
        for (int i = 0; i < nrows; i++) {
            sampleMat=Mat(1, nChars, CV_32FC1, {testChars.at<float>(i,0),testChars.at<float>(i,1),testChars.at<float>(i,2)});
            //cout << svm->getVarCount() << endl;
            result = svm->predict(sampleMat);
            cout << "Image " << i << ": " <<result<< endl;
        }*/
        /*
        float test[1][3]={542, 11, 4187};
        Mat sampleMat(1, nChars, CV_32FC1,test);
        float result = svm->predict(sampleMat);
        cout << "El resultado es: " << result << endl;*/
    }


    //svmFile.close();


    //svm->load("svm_result.xml");
    //vesselSegmentation(bgMask,maImage);



    //waitKey(0);
    return app.exec();
    //return 0;
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

    ifstream myReadFile(baseURL + "dataset.txt");
    if (myReadFile.is_open()) {
        while (myReadFile >> name >> type) {
            image = imread(baseURL + "dataset/" + name, 1);
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

void generateTestFile() {
    DIR *dir;
    struct dirent *ent;
    char *fileName;
    char fileType;

    if ((dir = opendir("image/0-dataset/test")) != NULL) {
        /* print all the files and directories within directory */
        ofstream outputFile("testset.txt");
        while ((ent = readdir(dir)) != NULL) {
            fileName = ent->d_name;
            fileType = fileName[0];
            switch (fileType) {
                case 'n':
                    outputFile << fileName << " " << "0" << endl;
                    break;
                case 'l':
                    outputFile << fileName << " " << "1" << endl;
                    break;
                case 'm':
                    outputFile << fileName << " " << "2" << endl;
                    break;
                case 's':
                    outputFile << fileName << " " << "3" << endl;
                    break;

            }
        }
        closedir(dir);
        outputFile.close();
    } else {
        /* could not open directory */
        cout << "Error opening directory..." << endl;
    }
}

void getCharsFromTestFolder(string& svmFileName) {
    //int numberOfImages = 100;
    int type;
    Mat image, bgMask, tmp;
    int flag = 1;
    CharsImage ci;
    int nImage = 0;
    string fileName = "";

    float result;
    Ptr<SVM> svm = SVM::create();
    svm = SVM::load<SVM>(svmFileName);
    cout << "Config SVM file opened..." << endl;
    float testSample[nChars];
    float acc = 0;
    int ncols = 2240, nrows = 1488;
    //VP=FP=VN=FN=0;
    int confMatrix [4][4] = {
        {0}
    };

    int debug = 1;
    if (!debug) {// verificar si se están realizando pruebas
        ifstream testDataFile("testset.txt");
        if (testDataFile.is_open()) {
            while (testDataFile >> fileName >> type) {
                ci.nImage = nImage;
                image = imread("image/0-dataset/test/" + fileName, 1);
                if (image.cols != ncols || image.rows != nrows) {
                    resize(image, image, Size(ncols, nrows));
                }
                if (flag) {
                   // backgroundSegmentation(image, bgMask);
                    readInGreenChannel("image/bgMask.tif",bgMask);
                    flag = 0;
                }
                //backgroundSegmentation(image, bgMask);
                bgMask.copyTo(tmp);
                
                opticDiscSegmentation(tmp, image, ci);
                //cout<<ci.nImage+i*numberOfImages<<endl;
                //imwrite("image/3-final mask/image" + SSTR(ci.nImage) + ".tif", tmp);
                darkLessionSegmentation(tmp, image, ci);
                brightLessionSegmentation(tmp, image, ci);
                //cout << "Imagen " + fileName + " " << nImage << ": " << ci.areaDarkZone << " " << ci.numberDarkZone << " " << ci.areaBrightZone << " " << ci.numberBrightZones << endl;
                testSample[0] = ci.areaDarkZone;
                testSample[1] = ci.numberDarkZone;
                testSample[2] = ci.areaBrightZone;
                testSample[3] = ci.numberBrightZones;
                Mat sampleMat(1, nChars, CV_32FC1, testSample);
                result = svm->predict(sampleMat);
                cout << "Imagen " << fileName << " " << nImage << ": (" << type << "," << result << ")" << endl;
                //if(nImage==20) return;
                confMatrix[type][(int) result]++;
                if (type == (int) result)
                    acc++;
                nImage++;
            }
        } else {
            /* could not open directory */
            cout << "Error opening test data file..." << endl;
        }
        testDataFile.close();
    } else {
        //ifstream trainChars("training_data.csv");

        cout << "Classification test mode" << endl;
        FILE *fp = fopen("training_data.csv", "r");
        int dArea, dNum, bArea, bNum;
        
        while (fscanf(fp, "%d,%d,%d,%d,%d",&dArea, &dNum, &bArea, &bNum, &type) != EOF) {
            testSample[0] = dArea;
            testSample[1] = dNum;
            testSample[2] = bArea;
            testSample[3] = bNum;
            Mat sampleMat(1, nChars, CV_32FC1, testSample);
            result = svm->predict(sampleMat);
            
            cout << "Imagen " << ": (" << type << "," << result << ")" << endl;
            //cout << name << " "<< dArea << " "<<dNum << " "<< bArea << " "<<bNum << " " << type << endl;
            if (type == (int) result)
                acc++;
            nImage++;
            confMatrix[type][(int) result]++;
        }
        fclose(fp);
    }
    cout << "Algorithm accuracy: " << acc / (numberOfImages*nCategories) << endl;
    printMatrix(confMatrix);
}

void printMatrix(int matrix[][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << matrix[i][j] << ' ';
        }
        cout << std::endl;
    }
}