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
int w=8;
//Scalar backgroundThreshold(0.734042, 0.982265, 1.42762);
Scalar backgroundThreshold(0, 0, 0);
Mat blankBlock(w,w,CV_8UC3, Scalar(255,255,255));
void backgroundSegmentation(const Mat& src, Mat dest){
    CV_Assert(src.depth() != sizeof(uchar));
    //int channels = src.channels();
    
    dest= Mat::zeros(src.rows,src.cols,src.type());
    //dest= Mat(src.rows,src.cols,src.type(),Scalar(255,0,0));
    int nRows = src.rows;
    int nCols = src.cols;
    /*if (src.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }*/
    int i,j;
    //uchar* p;
    nRows/=w;
    nCols/=w;
    Mat block;    
    //Vec3b stddev;
    Mat d;
    for( i = 0; i < nRows; i++)
    {
        //p = src.ptr<uchar>(i*w);
        for ( j = 0; j < nCols; j++)
        {
            block= Mat(src,Rect(j*w,i*w,w,w));
            d=Mat(dest,Rect(j*w,i*w,w,w));
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") stddev:"<<calculateStdDev(block)<<endl;
            //retrieveBackground(d,calculateStdDev(block));
            retrieveBackground(d,mean(block));
            //cout<<"Bloque ("<<j+1<<","<<i+1<<") prom:"<<mean(block)<<endl;
        }
    }
    
    //dilate
    Mat result;
    src.copyTo(result,dest);
    imwrite("img/background_mask.tif",dest);
    imwrite("img/result.tif",result);
    /*namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
    namedWindow("BackGround Segmentation", CV_WINDOW_AUTOSIZE);
    imshow("Original Image",src);
    imshow("BackGround Segmentation",dest);*/
    
}

void retrieveBackground(Mat dest,Scalar stdDev){
    /*if(
            stdDev.val[0] >= backgroundThreshold.val[0] ||
            stdDev.val[1] >= backgroundThreshold.val[1] ||
            stdDev.val[2] >= backgroundThreshold.val[2]
            ){
        blankBlock.copyTo(dest);
    }*/
    
    if(calculateDistance(stdDev)>=7.3){//7.3
        blankBlock.copyTo(dest);
    }
}
double calculateDistance(Scalar stdDev){
    double x= stdDev.val[0] - backgroundThreshold.val[0];
    double y=stdDev.val[1] - backgroundThreshold.val[1];
    double z=stdDev.val[2] - backgroundThreshold.val[2];
    
    return sqrt(x*x+y*y+z*z);
}
Scalar calculateStdDev(Mat& block){
    Scalar m=mean(block);
    Scalar stdDev;
    int nRows = block.rows;
    int nCols = block.cols;
    
    Vec3b* ptr;
    double aux;
    double sumB=0;
    double sumG=0;
    double sumR=0;
    for(int i=0;i<nRows;i++){
        ptr=block.ptr<Vec3b>(i);        
        for(int j=0;j<nCols;j++){
            aux=ptr[j].val[0]-m.val[0];
            aux=aux*aux;
            sumB+=aux;
            
            aux=ptr[j].val[1]-m.val[1];
            aux=aux*aux;
            sumG+=aux;
            
            aux=ptr[j].val[2]-m.val[2];
            aux=aux*aux;
            sumR+=aux;
        }
    }
    sumB=sumB/(w*w);
    stdDev.val[0]=sqrt(sumB);
    
    sumG=sumG/(w*w);
    stdDev.val[1]=sqrt(sumG);
    
    sumR=sumR/(w*w);
    stdDev.val[2]=sqrt(sumR);
    
    return stdDev;
}




