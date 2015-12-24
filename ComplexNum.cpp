/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComplexNum.cpp
 * Author: dabarca
 * 
 * Created on 23 de diciembre de 2015, 12:24 PM
 */

#include "ComplexNum.h"
#include "math.h"
ComplexNum::ComplexNum() {
}

ComplexNum::ComplexNum(const ComplexNum& orig) {    
    this->imag=orig.imag;
}
ComplexNum::~ComplexNum(){
    
}
ComplexNum::ComplexNum(float a){
    imag=Mat(2,1,CV_32FC1);
    imag.ptr<float>(0)[0]=a;
    imag.ptr<float>(1)[0]=0;
}
ComplexNum::ComplexNum(Mat a){
    imag=a;
    /*imag.ptr<float>(0)[0]=a.ptr<float>(0)[0];
    imag.ptr<float>(1)[0]=a.ptr<float>(1)[0];*/
}

ComplexNum::ComplexNum(float a, float b) {
    imag=Mat(2,1,CV_32FC1);
    imag.ptr<float>(0)[0]=a;
    imag.ptr<float>(1)[0]=b;
    
}

float ComplexNum::getReal(){
    return this->imag.ptr<float>(0)[0];
}

float ComplexNum::getImg(){
    return this->imag.ptr<float>(1)[0];
}

ComplexNum ComplexNum::operator+(ComplexNum& b) {
    this->imag.ptr<float>(0)[0] += b.imag.ptr<float>(0)[0];
    this->imag.ptr<float>(1)[0] += b.imag.ptr<float>(1)[0];
    return *this;
}

ComplexNum ComplexNum::operator-(ComplexNum& b) {
    this->imag.ptr<float>(0)[0] -= b.imag.ptr<float>(0)[0];
    this->imag.ptr<float>(1)[0] -= b.imag.ptr<float>(1)[0];
    return *this;
}

ComplexNum ComplexNum::operator*(ComplexNum& x) {
    int a = this->imag.ptr<float>(0)[0];
    int b = this->imag.ptr<float>(1)[0];
    int c = x.imag.ptr<float>(0)[0];
    int d = x.imag.ptr<float>(1)[0];

    this->imag.ptr<float>(0)[0] = a * c - b*d;
    this->imag.ptr<float>(1)[0] = a * d + b*c;
    return *this;
}

ComplexNum ComplexNum::operator*(float b) {
    this->imag.ptr<float>(0)[0]*=b;
    this->imag.ptr<float>(1)[0]*=b;
    return *this;
}
ComplexNum ComplexNum::complexExp(){
    float a = this->imag.ptr<float>(0)[0];
    float b = this->imag.ptr<float>(1)[0];
    this->imag.ptr<float>(0)[0] = exp(a) * cos(b); //real part
    this->imag.ptr<float>(1)[0] = exp(a) * sin(b); //img part
    return *this;
}


ComplexNum ComplexNum::operator = (ComplexNum& b){
    this->imag.ptr<float>(0)[0] = b.imag.ptr<float>(0)[0];
    this->imag.ptr<float>(1)[0] = b.imag.ptr<float>(1)[0];
    return *this;
}
/*
Mat ComplexNum::operator *(Mat matrix){
    int nrows = matrix.rows;
    int ncols = matrix.cols;
    Vec2f* ptr;
    

    for (int i = 0; i < nrows; i++) {
        ptr = matrix.ptr<Vec2f>(i);
        for (int j = 0; i < ncols; i++) {
            ComplexNum b(ptr[j]);
            ComplexNum c=(*this)*b;
            ptr[j][0]=c.imag[0];
            ptr[j][1]=c.imag[1];
        }
    }

    return matrix;
}*/

ComplexNum ComplexNum::conj(){
    this->imag.ptr<float>(1)[0]*=-1;
    return *this;
}


Mat ComplexNum::toMat(){
       
    return this->imag;
}
void ComplexNum::printComplexNum(){
    cout<<"["<<this->imag.ptr<float>(0)[0]<<","<<this->imag.ptr<float>(1)[0]<<"]"<<endl;
}
