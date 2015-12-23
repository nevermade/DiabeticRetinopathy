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
using namespace cv;
class ComplexNum {
    Vec2f imag;
public:    
    ComplexNum();
    ComplexNum(float a, float b);
    ComplexNum(float a);
    ComplexNum(Vec2f a);
    ComplexNum(const ComplexNum& orig);
    virtual ~ComplexNum();
    ComplexNum operator + (ComplexNum b);
    ComplexNum operator - (ComplexNum b);
    ComplexNum operator * (ComplexNum b);
    ComplexNum operator * (float b);
    Mat operator * (Mat m);
    ComplexNum operator = (ComplexNum b);
    ComplexNum complexExp();
    ComplexNum conj();
    float getReal();
    float getImg();
private:
    
};

#endif /* COMPLEX_H */

