/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   util.cpp
 * Author: espri
 * 
 * Created on 3 de diciembre de 2015, 02:17 PM
 */
#include <cstdlib>
#include "preprocessing.h"
using namespace std;
int w = 8;
int morph_element_size = 5;

//Scalar backgroundThreshold(0.734042, 0.982265, 1.42762);
Scalar backgroundThreshold(0, 0, 0);
Mat blankBlock(w, w, CV_8UC3, Scalar(255, 255, 255));

void backgroundSegmentation(const Mat& src, Mat& bgMask) {
    CV_Assert(src.depth() != sizeof (uchar));
    bgMask = Mat::zeros(src.rows, src.cols, src.type());
    int nRows = src.rows;
    int nCols = src.cols;
    nRows /= w;
    nCols /= w;
    Mat block;
    //Vec3b stddev;
    Mat d;
    for (int i = 0; i < nRows; i++) {
        //p = src.ptr<uchar>(i*w);
        for (int j = 0; j < nCols; j++) {
            block = Mat(src, Rect(j*w, i*w, w, w));
            d = Mat(bgMask, Rect(j*w, i*w, w, w));
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") stddev:"<<calculateStdDev(block)<<endl;          
            retrieveBackgroundMask(block, d); // mean(block));            
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") prom:"<<mean(block)<<endl;
        }
    }
    //fineBackGroundSegmentation(dest);
    Mat backgroundImageResult;
    medianBlur(bgMask, bgMask, 3);
    
    vector<Mat> channels;
    split(bgMask, channels);
    bgMask = channels[1];

    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(bgMask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<vector<Point> > contours_poly(contours.size());
    vector<Point2f>center(contours.size());
    vector<float>radius(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        minEnclosingCircle((Mat) contours_poly[i], center[i], radius[i]);
    }
     Mat fineMask = Mat::zeros(bgMask.rows, bgMask.cols, CV_8UC1);
    for( int i = 0; i< contours.size(); i++ )
     {
       circle(fineMask, center[i], radius[i]-67, Scalar(255), -1, 8, 0);
     }
    
    
    bgMask = fineMask;
    src.copyTo(backgroundImageResult, bgMask);
    imwrite("image/1-background/image1.tif", backgroundImageResult);
    imwrite("image/1-background/mask1.tif", bgMask);
}
//Se aplica la dilatacion de la mascara segmentada del fondo

void fineBackGroundSegmentation(Mat& dest) {
    Mat element = getStructuringElement(MORPH_RECT, Size(morph_element_size, morph_element_size));
    dilate(dest, dest, element);

    namedWindow("Dilation Demo", CV_WINDOW_AUTOSIZE);
    imshow("Dilation Demo", dest);
}

void noiseSegmentation(const Mat& src, Mat& dest) {
    CV_Assert(src.depth() != sizeof (uchar));
    dest = Mat::zeros(src.rows, src.cols, src.type());
    int nRows = src.rows;
    int nCols = src.cols;
    nRows /= w;
    nCols /= w;
    Mat block;
    //Equalization
    Mat eqH(src.rows, src.cols, src.type()), medianFilter, ycrcb, d, hsi;
    cvtColor(src, ycrcb, CV_BGR2YCrCb);
    vector<Mat> channels;
    split(ycrcb, channels);
    equalizeHist(channels[0], channels[0]);
    merge(channels, ycrcb);
    cvtColor(ycrcb, eqH, CV_YCrCb2BGR);
    medianBlur(eqH, medianFilter, 3);
    bgrToHsi(medianFilter, hsi);
    //imwrite("img/noise_hsi.tif", hsi);    
    //imwrite("img/median_filter.tif", medianFilter);

    for (int i = 0; i < nRows; i++) {
        //p = src.ptr<uchar>(i*w);
        for (int j = 0; j < nCols; j++) {
            block = Mat(hsi, Rect(j*w, i*w, w, w));
            d = Mat(dest, Rect(j*w, i*w, w, w));
            retrieveNoiseMask(block, d);
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") prom:"<<calculateNoiseFactor(block)<<endl;
        }
    }
    fineNoiseSegmentation(dest);
}

void fineNoiseSegmentation(Mat& dest) {
    Mat element = getStructuringElement(MORPH_RECT, Size(morph_element_size, morph_element_size));
    morphologyEx(dest, dest, MORPH_OPEN, element);
    erode(dest, dest, element);
    /*namedWindow("Fine Segmentation Noise Mask", CV_WINDOW_AUTOSIZE);
    imshow( "Fine Segmentation Noise Mask", dest);*/
}

void retrieveNoiseMask(Mat src, Mat dest) {
    if (calculateNoiseFactor(src) <= 0.315) {
        blankBlock.copyTo(dest);
    }
    //cout<<"Bloque ("<<j+1<<","<<i+1<<") prom:"<<calculateNoiseFactor(dest)<<endl;
}

void retrieveBackgroundMask(Mat src, Mat dest) {
    /*if(
            stdDev.val[0] >= backgroundThreshold.val[0] ||
            stdDev.val[1] >= backgroundThreshold.val[1] ||
            stdDev.val[2] >= backgroundThreshold.val[2]
            ){
        blankBlock.copyTo(dest);
    }*/

    if (calculateDistance(mean(src)) >= 7.3) {//7.3
        blankBlock.copyTo(dest);
    }

    /*if (stdDev.val[2] >= 1.9) {//7.3
        blankBlock.copyTo(dest);
    }*/
}

double calculateDistance(Scalar mean) {
    double x = mean.val[0] - backgroundThreshold.val[0];
    double y = mean.val[1] - backgroundThreshold.val[1];
    double z = mean.val[2] - backgroundThreshold.val[2];

    return sqrt(x * x + y * y + z * z);
}

Scalar calculateStdDev(Mat& block) {
    Scalar m = mean(block);
    Scalar stdDev;
    int nRows = block.rows;
    int nCols = block.cols;

    Vec3b* ptr;
    double aux;
    double sumB = 0;
    double sumG = 0;
    double sumR = 0;
    for (int i = 0; i < nRows; i++) {
        ptr = block.ptr<Vec3b>(i);
        for (int j = 0; j < nCols; j++) {
            aux = ptr[j].val[0] - m.val[0];
            aux = aux*aux;
            sumB += aux;

            aux = ptr[j].val[1] - m.val[1];
            aux = aux*aux;
            sumG += aux;

            aux = ptr[j].val[2] - m.val[2];
            aux = aux*aux;
            sumR += aux;
        }
    }
    sumB = sumB / (w * w);
    stdDev.val[0] = sqrt(sumB);

    sumG = sumG / (w * w);
    stdDev.val[1] = sqrt(sumG);

    sumR = sumR / (w * w);
    stdDev.val[2] = sqrt(sumR);

    return stdDev;
}

void bgrToHsi(Mat src, Mat& hsi) {
    hsi = Mat(src.rows, src.cols, src.type());
    double r, g, b, h, s, in;
    Vec3b* p, *pHsi;
    for (int i = 0; i < src.rows; i++) {

        p = src.ptr<Vec3b>(i);
        pHsi = hsi.ptr<Vec3b>(i);
        for (int j = 0; j < src.cols; j++) {
            b = p[j].val[0];
            g = p[j].val[1];
            r = p[j].val[2];

            in = (b + g + r) / 3;

            if (b == g && g == r) {
                h = s = 0;
            } else {
                int min_val = 0;
                min_val = std::min(r, std::min(b, g));

                s = 1 - 3 * (min_val / (b + g + r));

                if (s < 0.00001) {
                    s = 0;
                } else if (s > 0.99999) {
                    s = 1;
                }

                if (s != 0) {
                    //h = (r - 0.5 * g - 0.5 * b) / sqrt(r * r + g * g + b * b - r * g - r * b - g * b);
                    h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
                    h = acos(h);
                    //cout <<"i:"<< i <<"j:"<<j<< endl;                   
                    if (b <= g) {
                        h = h;
                    } else {
                        h = ((360 * 3.14159265) / 180.0) - h;
                    }
                }
            }
            pHsi[j].val[0] = (h * 180) / 3.14159265; //H
            pHsi[j].val[1] = s; //S
            pHsi[j].val[2] = in; //I
            /*hsi.at<Vec3b>(i, j)[0] = (h * 180) / 3.14159265;
            hsi.at<Vec3b>(i, j)[1] = s*100;
            hsi.at<Vec3b>(i, j)[2] = in;*/
        }
    }

}


//Esta funcion calcula el ruido de la imagen HSI

double calculateNoiseFactor(Mat& block) {
    double noiseFactorTotal = 0, pixelNoise;
    int nRows = block.rows;
    int nCols = block.cols;
    Vec3b *ptr;
    for (int i = 0; i < nRows; i++) {
        ptr = block.ptr<Vec3b>(i);
        for (int j = 0; j < nCols; j++) {
            if (ptr[j].val[0] == 0 || ptr[j].val[2] == 0)
                continue;
            pixelNoise = (double) ptr[j].val[0] / ptr[j].val[2]; // h/i
            noiseFactorTotal += pixelNoise;
        }
    }
    noiseFactorTotal /= (w * w);
    return noiseFactorTotal;
}

void finalSegmentation(const Mat& src, Mat& bgMask, Mat& noiseMask, Mat& finalMask, Mat& result) {

    bitwise_and(bgMask, noiseMask, finalMask);
    src.copyTo(result, finalMask);
    imwrite("img/final_result.tif", result);

}



