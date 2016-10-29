/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainForm.h
 * Author: USER
 *
 * Created on 16 de octubre de 2016, 10:19 PM
 */

#ifndef _MAINFORM_H
#define _MAINFORM_H

#include "ui_MainForm.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "preprocessing.h"
#include "feature_extraction.h"
using namespace std;
using namespace cv;
using namespace cv::ml;

class MainForm : public QDialog {
    Q_OBJECT
public:
    MainForm();
    virtual ~MainForm();

public slots:
    void clickBtnSearch();
    void classify();
    
    
private:
    Ui::MainForm widget;
    Ptr<SVM> svm;
    void showEvent( QShowEvent* event );
    Mat retina;
};

#endif /* _MAINFORM_H */
