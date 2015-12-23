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
using namespace cv;
void imageVesselEnhancement(Mat& result,Mat& finalMask);
void fourierTransform(const Mat& invGC, Mat& complexI);
void apply2DCWT(Mat& complexI);
void printMatrix(const Mat& m);
template<typename _Tp>
void printMatrix(Mat& m);
Mat apply2DMorletWavelet(ComplexNum K, Mat inv);
Mat matrixComplexExp(Mat matrix);

#endif /* FEATURE_EXTRACTION_H */

