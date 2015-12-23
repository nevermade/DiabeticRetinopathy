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
}
ComplexNum::~ComplexNum(){
    
}
ComplexNum::ComplexNum(float a){
    imag[0]=a;
    imag[1]=0;
}
ComplexNum::ComplexNum(Vec2f a){
    imag[0]=a[0];
    imag[1]=a[1];
}

ComplexNum::ComplexNum(float a, float b) {
    imag[0] = a;
    imag[1] = b;
    
}

float ComplexNum::getReal(){
    return this->imag[0];
}

float ComplexNum::getImg(){
    return this->imag[1];
}

ComplexNum ComplexNum::operator+(ComplexNum b) {
    this->imag[0] += b.imag[0];
    this->imag[1] += b.imag[1];
    return *this;
}

ComplexNum ComplexNum::operator-(ComplexNum b) {
    this->imag[0] -= b.imag[0];
    this->imag[1] -= b.imag[1];
    return *this;
}

ComplexNum ComplexNum::operator*(ComplexNum x) {
    int a = this->imag[0];
    int b = this->imag[1];
    int c = x.imag[0];
    int d = x.imag[1];

    this->imag[0] = a * c - b*d;
    this->imag[0] = a * d + b*c;
    return *this;
}

ComplexNum ComplexNum::operator*(float b) {
    this->imag[0]*=b;
    this->imag[1]*=b;
    return *this;
}
ComplexNum ComplexNum::complexExp(){
    float a = this->imag[0];
    float b = this->imag[1];
    this->imag[0] = exp(a) * cos(b); //real part
    this->imag[1] = exp(a) * sin(b); //img part
}


ComplexNum ComplexNum::operator = (ComplexNum b){
    this->imag[0]=b.imag[0];
    this->imag[1]=b.imag[1];
    return *this;
}

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
}

ComplexNum ComplexNum::conj(){
    this->imag[1]*=-1;
    return *this;
}