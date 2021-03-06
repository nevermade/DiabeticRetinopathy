/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   util.h
 * Author: espri
 *
 * Created on 3 de diciembre de 2015, 02:17 PM
 */

#ifndef PREPROCESSING_H
#define PREPROCESSING_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
void backgroundSegmentation(Mat& src, Mat& dest);
Scalar calculateStdDev(Mat& block);
void retrieveBackgroundMask(Mat src, Mat dest);
double calculateDistance(Scalar mean);
void  bgrToHsi(Mat src, Mat& dest);
double calculateNoiseFactor(Mat& block);
void noiseSegmentation(const Mat& src, Mat& dest);
void retrieveNoiseMask(Mat src, Mat dest);
void fineBackGroundSegmentation(Mat& dest);
void fineNoiseSegmentation(Mat& dest);
void finalSegmentation(const Mat& src,Mat& bgMask, Mat& noiseMask,Mat& finalMask, Mat& result);

#endif /* UTIL_H */

