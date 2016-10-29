/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   MainForm.cpp
 * Author: USER
 *
 * Created on 16 de octubre de 2016, 10:19 PM
 */

#include "MainForm.h"
#include "QFileDialog"

MainForm::MainForm() {
    widget.setupUi(this);
    //connect(widget.btnSearch, SIGNAL(clicked()), this, SLOT(clickBtnSearch()));
    //connect(this, SIGNAL(window_loaded), this, SLOT(onLoadWindow()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
}

MainForm::~MainForm() {

}

void MainForm::clickBtnSearch() {
    QString path = QFileDialog::getOpenFileName(this, "Seleccionar imagen .tif (*.tif)", "./", "(*.tif)");
    if (path.isNull() == false) {
        widget.txtPath->setText(path);
        this->retina = imread(path.toStdString(), 1);
        /*CharsImage ci;
        int nChars=4;
        float testSample[nChars];
        float result;
        
        Mat image = imread(path.toStdString(), 1);
        Mat bgMask,tmp;
        readInGreenChannel("image/bgMask.tif", bgMask);
        
        //backgroundSegmentation(image, bgMask);
        bgMask.copyTo(tmp);

        opticDiscSegmentation(tmp, image, ci);
        //cout<<ci.nImage+i*numberOfImages<<endl;
        //imwrite("image/3-final mask/image" + SSTR(ci.nImage) + ".tif", tmp);
        darkLessionSegmentation(tmp, image, ci);
        brightLessionSegmentation(tmp, image, ci);
        //cout << "Imagen " + fileName + " " << nImage << ": " << ci.areaDarkZone << " " << ci.numberDarkZone << " " << ci.areaBrightZone << " " << ci.numberBrightZones << endl;
        testSample[0] = ci.areaDarkZone;
        testSample[1] = ci.numberDarkZone;
        testSample[2] = ci.areaBrightZone;
        testSample[3] = ci.numberBrightZones;
        Mat sampleMat(1, nChars, CV_32FC1, testSample);
        result = svm->predict(sampleMat);*/

        QGraphicsScene *scene;
        QPixmap image, resize;
        image.load(path);
        //QImage image("test.png");

        scene = new QGraphicsScene(this);
        resize = image.scaled(widget.canvasImage->width(), widget.canvasImage->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        scene->addPixmap(resize);
        scene->setSceneRect(resize.rect());
        widget.canvasImage->setScene(scene);
        //QGraphicsPixmapItem item( QPixmap::fromImage(image));
        widget.txtResult->setText("");
    }
}

void MainForm::showEvent(QShowEvent* event) {
    string svmFileName = "svm_training.xml";
    svm = SVM::create();
    svm = SVM::load<SVM>(svmFileName);
    cout << "se cargo la ventana" << endl;
    widget.txtResult->setText("");
}

void MainForm::classify() {
    int nChars = 4;
    float testSample[nChars];
    float result;
    CharsImage ci;
    Mat image = this->retina;
    if (image.cols != 2240 || image.rows != 1488) {
        cv::resize(image, image, Size(2240, 1488));
    }
    Mat bgMask, tmp;
    readInGreenChannel("image/bgMask.tif", bgMask);

    //backgroundSegmentation(image, bgMask);
    bgMask.copyTo(tmp);

    opticDiscSegmentation(tmp, image, ci);
    //cout<<ci.nImage+i*numberOfImages<<endl;
    //imwrite("image/3-final mask/image" + SSTR(ci.nImage) + ".tif", tmp);
    darkLessionSegmentation(tmp, image, ci);
    brightLessionSegmentation(tmp, image, ci);
    //cout << "Imagen " + fileName + " " << nImage << ": " << ci.areaDarkZone << " " << ci.numberDarkZone << " " << ci.areaBrightZone << " " << ci.numberBrightZones << endl;
    testSample[0] = ci.areaDarkZone;
    testSample[1] = ci.numberDarkZone;
    testSample[2] = ci.areaBrightZone;
    testSample[3] = ci.numberBrightZones;
    Mat sampleMat(1, nChars, CV_32FC1, testSample);
    result = svm->predict(sampleMat);

    switch ((int) result) {
        case 0:
            widget.txtResult->setText("Normal");
            break;
        case 1:
            widget.txtResult->setText("Leve");
            break;
        case 2:
            widget.txtResult->setText("Moderado");
            break;
        case 3:
            widget.txtResult->setText("Severo");
            break;
    }
}