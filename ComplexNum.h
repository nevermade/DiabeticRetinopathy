/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComplexNum.h
 * Author: dabarca
 *
 * Created on 23 de diciembre de 2015, 12:24 PM
 */

#ifndef COMPLEXNUM_H
#define COMPLEXNUM_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <limits>
using namespace cv;
using namespace std;
class ComplexNum {
    Mat imag;
public:    
    ComplexNum();
    ComplexNum(double a, double b);
    ComplexNum(double a);
    
    ComplexNum(Mat& a);
    ComplexNum(const ComplexNum& orig);
    virtual ~ComplexNum();
    ComplexNum operator + (const ComplexNum& b);
    ComplexNum operator - (const ComplexNum& b);
    ComplexNum operator * (const ComplexNum& b);
    ComplexNum operator * (double b);
   // Mat operator * (Mat m);
    void operator = (const ComplexNum &b);
    ComplexNum complexExp();
    ComplexNum conj();
    Mat toMat();    
    double getReal();
    double getImg();
    void printComplexNum();
private:
    
};

#endif /* COMPLEX_H */

