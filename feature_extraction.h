/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   feature_extraction.h
 * Author: espri
 *
 * Created on 16 de diciembre de 2015, 08:02 PM
 */

#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
void opticDiscSegmentation(Mat& bgMask, Mat& maImage);
void darkLessionSegmentation(Mat& bgMask, Mat& maImage);
void brightLessionSegmentation();
double calculateHThreshold(Mat &image);
double calculateMean(Mat& image);
int calculateMedian(Mat& image, Mat& mask);
void vesselSegmentation(Mat& bgMask,Mat& maImage);
void getLinePoints(int l,Point &start, Point &end, double theta);
void getOrtogonalLinePoints(int l, Point &start, Point &end, double theta);
double calculateLineStrength(Mat &img, vector<Point> &it);
double calculateLineStrength(Mat &img, Point it[]);
vector<vector<Point> > calculateLineIterators(int l, int step);
void calculateOrtLineIterators(Point it[][3],int l, int step);
//vector<vector<Point> > 
vector<Point> getDDALine(Point &start, Point &end);
double getLineResponse(Mat &square, vector<vector<Point> > &lineIt, Point it[][3]);
void iluminationEqualization(Mat& input, Mat& output, Mat& mask);
void getLineImageResponse(Mat& input, Mat& output, Mat& mask, int lineLength, int lineStep);

/**util**/

void readBinaryInBinary(String path, Mat& binary);
void readInGreenChannel(const String& path, Mat& image);

#endif /* FEATURE_EXTRACTION_H */

