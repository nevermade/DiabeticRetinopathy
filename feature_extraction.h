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
#include "ComplexNum.h"
#include <math.h>
using namespace cv;
void imageVesselEnhancement(Mat& result,Mat& finalMask);
void fourierTransform(const Mat& invGC, Mat& complexI);
void apply2DCWT(Mat& complexI);
void printMatrix(const Mat& m);
template<typename _Tp>
void printMatrix(Mat& m);
void apply2DMorletWavelet(ComplexNum& K, Mat& inv, Mat& morletWavelet);
void matrixComplexExp(Mat& matrix);
ComplexNum toComplexNum(Mat& a);
Mat getRotationMatrix(double theta);
void opticDiscSegmentation();
void darkLessionSegmentation();
void brightLessionSegmentation();
double calculateHThreshold(Mat &image);
double calculateMean(Mat& image);
int calculateMedian(Mat& image);
void vesselSegmentation();
void getLinePoints(int l,Point &start, Point &end, double theta);
void getOrtogonalLinePoints(int l, Point &start, Point &end, double theta);
double calculateLineStrength(Mat &img, vector<Point> &it);
vector<vector<Point> > calculateLineIterators(int l, int step, Mat& m);
vector<vector<Point> > calculateOrtLineIterators(int l, int step, Mat& m);
//vector<vector<Point> > 
vector<Point> getBSHLine(Point &start, Point &end);
#endif /* FEATURE_EXTRACTION_H */

