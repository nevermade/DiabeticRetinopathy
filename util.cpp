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
#include "util.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
int w = 8;
//Scalar backgroundThreshold(0.734042, 0.982265, 1.42762);
Scalar backgroundThreshold(0, 0, 0);
Mat blankBlock(w, w, CV_8UC3, Scalar(255, 255, 255));

void backgroundSegmentation(const Mat& src, Mat dest) {
    CV_Assert(src.depth() != sizeof (uchar));
    //int channels = src.channels();

    dest = Mat::zeros(src.rows, src.cols, src.type());
    //dest= Mat(src.rows,src.cols,src.type(),Scalar(255,0,0));
    int nRows = src.rows;
    int nCols = src.cols;
    /*if (src.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }*/
    int i, j;
    //uchar* p;
    nRows /= w;
    nCols /= w;
    Mat block;
    //Vec3b stddev;
    Mat d;
    for (i = 0; i < nRows; i++) {
        //p = src.ptr<uchar>(i*w);
        for (j = 0; j < nCols; j++) {
            block = Mat(src, Rect(j*w, i*w, w, w));
            d = Mat(dest, Rect(j*w, i*w, w, w));
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") stddev:"<<calculateStdDev(block)<<endl;
            //retrieveBackground(d,calculateStdDev(block));
            retrieveBackground(d, mean(block));
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") prom:"<<mean(block)<<endl;
        }
    }

    //dilate
    Mat result, eqH(src.rows, src.cols, src.type()), gray, medianFilter;
    src.copyTo(result, dest);
    imwrite("img/background_mask.tif", dest);
    imwrite("img/result.tif", result);

    Mat ycrcb;
    cvtColor(src, ycrcb, CV_BGR2YCrCb);

    vector<Mat> channels;
    split(ycrcb, channels);

    equalizeHist(channels[0], channels[0]);

    Mat result2;
    merge(channels, ycrcb);
    cvtColor(ycrcb, eqH, CV_YCrCb2BGR);

    /*cvtColor(src, gray, CV_BGR2GRAY);
    equalizeHist(gray, eqH);*/
    imwrite("img/equalized_histogram.tif", eqH);
    medianBlur(eqH, medianFilter, 3);
    imwrite("img/median_filter.tif", medianFilter);
    /*namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
    namedWindow("BackGround Segmentation", CV_WINDOW_AUTOSIZE);
    imshow("Original Image",src);
    imshow("BackGround Segmentation",dest);*/

}

void retrieveBackground(Mat dest, Scalar stdDev) {
    /*if(
            stdDev.val[0] >= backgroundThreshold.val[0] ||
            stdDev.val[1] >= backgroundThreshold.val[1] ||
            stdDev.val[2] >= backgroundThreshold.val[2]
            ){
        blankBlock.copyTo(dest);
    }*/

    if (calculateDistance(stdDev) >= 7.3) {//7.3
        blankBlock.copyTo(dest);
    }

    /*if (stdDev.val[2] >= 1.9) {//7.3
        blankBlock.copyTo(dest);
    }*/
}

double calculateDistance(Scalar stdDev) {
    double x = stdDev.val[0] - backgroundThreshold.val[0];
    double y = stdDev.val[1] - backgroundThreshold.val[1];
    double z = stdDev.val[2] - backgroundThreshold.val[2];

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
                    h = (r - 0.5 * g - 0.5 * b) / sqrt(r * r + g * g + b * b - r * g - r * b - g * b);
                    //h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
                    h = acos(h);
                    //cout <<"i:"<< i <<"j:"<<j<< endl;                   
                    if (b <= g) {
                        h = h;
                    } else {
                        h = ((360 * 3.14159265) / 180.0) - h;
                    }
                }
            }
            pHsi[j].val[0] = (h * 180) / 3.14159265;
            pHsi[j].val[1] = s * 100;
            pHsi[j].val[2] = in;
            /*hsi.at<Vec3b>(i, j)[0] = (h * 180) / 3.14159265;
            hsi.at<Vec3b>(i, j)[1] = s*100;
            hsi.at<Vec3b>(i, j)[2] = in;*/
        }
    }

}




