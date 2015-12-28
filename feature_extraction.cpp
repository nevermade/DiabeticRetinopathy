/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cstdlib>
#include "feature_extraction.h"
#include "util.h"

using namespace std;

double step = 0.2618, max = 2.8798, dilation = 3, elongation = 4, theta = 0;

void imageVesselEnhancement(Mat& result, Mat& finalMask) {

    vector<Mat> channels;
    vector<Mat> maskChannels;
    split(finalMask, maskChannels);
    split(result, channels);
    bitwise_not(channels[1], channels[1], maskChannels[1]);
    imwrite("img/green_inverted_image.tiff", channels[1]);
    Mat complexI, invGC = channels[1];
    //Fourier transform
    //Mat planes[2];

    fourierTransform(invGC, complexI);
    apply2DCWT(complexI);

}

void fourierTransform(const Mat& invGC, Mat& complexI) {
    Mat padded; //expand input image to optimal size
    int m = getOptimalDFTSize(invGC.rows);
    int n = getOptimalDFTSize(invGC.cols); // on the border add zero values
    copyMakeBorder(invGC, padded, 0, m - invGC.rows, 0, n - invGC.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<double>(padded), Mat::zeros(padded.size(), CV_64F)};
    //Mat complexI;
    merge(planes, 2, complexI); // Add to the expanded another plane with zeros

    dft(complexI, complexI); // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]); // planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1); // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy)); // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy)); // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp; // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with double values into a
    // viewable image form (double between values 0 and 1).

    // imshow("Input Image"       , invGC   );    // Show the result
    //imshow("spectrum magnitude", magI);
    //imwrite("img/fourier_transform.tiff",magI);
    //waitKey();

    //return 0;

}

Mat getRotationMatrix(double theta){
    Mat rot(2,2,CV_32FC1);
    rot.ptr<double>(0)[0] = cos(theta);
    rot.ptr<double>(0)[1] = -sin(theta);
    rot.ptr<double>(1)[0] = sin(theta);
    rot.ptr<double>(1)[1] = cos(theta);
    return rot;
}
void apply2DCWT(Mat& complexI) {


    Mat A(2, 2, CV_32FC1);
    Mat rotMtrx;//rotation matrix
    
    //Set A
    A.ptr<double>(0)[0] = pow(elongation, -0.5);
    A.ptr<double>(0)[1] = 0;
    A.ptr<double>(1)[0] = 0;
    A.ptr<double>(1)[1] = 1;
    
    //Set rotation matrix
    rotMtrx=getRotationMatrix(-theta);    
    //A.inv();
    Mat inv = A.inv(), morletWavelet;
    /*ComplexNum K(complexI.ptr<double>(0)[0], complexI.ptr<double>(0)[1]);

    K.printComplexNum();*/
    //morletWavelet = toComplexNum(apply2DMorletWavelet(K, inv)).conj().toMat();//Apply conjugate
    
    /*Mat complexExponential;
    ComplexNum complexUnit(0,1);
    
    complexExponential=((ComplexNum)(complexUnit*K)).toMat();*/
    
    int nrows=complexI.rows;
    int ncols=complexI.cols;
    Vec2d *ptr;
    
    for(int i=500;;i++){
        ptr=complexI.ptr<Vec2d>(i);
        for(int j=0;j<ncols;j++){
            ComplexNum K(ptr[j][0],ptr[j][1]);
            //K.printComplexNum();
            apply2DMorletWavelet(K, inv,morletWavelet);
            toComplexNum(morletWavelet).conj().toMat();
            printMatrix<double>(morletWavelet);
        }
        
    }
    
    //apply conjugate
    /* morletWavelet.ptr<double>(0)[1] *= -1;

     //imaginary uni j
     Mat j(2, 1, CV_32FC1);
     j.ptr<double>(0)[0] = 0;
     j.ptr<double>(1)[0] = 1;
     //displacement vector b
     Mat b(2, 1, CV_32FC1, Scalar(0));
     b.ptr<double>(0)[0] = 0;
     b.ptr<double>(1)[0] = 0;

     Mat cwt;
     cwt = j*K;
     complexExp(cwt)*/;

}

template<typename _Tp>
void printMatrix(Mat& m) {
    int rows = m.rows;
    int cols = m.cols;
    cout << "---" << endl;
    for (int i = 0; i < rows; i++) {
        _Tp* ptr = m.ptr<_Tp>(i);
        for (int j = 0; j < cols; j++) {
            cout << ptr[j] << " ";
        }
        cout << endl;
    }
}

void apply2DMorletWavelet(ComplexNum& K, Mat& inv, Mat& morletWavelet) {    
    ComplexNum K0(0, 3),temp;
    //K0.printComplexNum();
    temp=K-K0;
    //temp.printComplexNum();
    temp=temp*temp;
    morletWavelet = temp.toMat();
    //printMatrix<double>(morletWavelet);
    //printMatrix<double>(inv);
    //printMatrix<double>(morletWavelet);
    morletWavelet =-0.5* (inv * morletWavelet);
    //printMatrix<double>(morletWavelet);
    temp = toComplexNum(morletWavelet);
    temp= temp.complexExp();
    morletWavelet=temp.toMat()*sqrt(elongation);
    //printMatrix<double>(morletWavelet);    
    
}

void matrixComplexExp(Mat& matrix) {
    int nrows = matrix.rows;
    int ncols = matrix.cols;
    Vec2d* ptr;
    for (int i = 0; i < nrows; i++) {
        ptr = matrix.ptr<Vec2d>(i);
        for (int j = 0; i < ncols; i++) {
            ComplexNum a(ptr[j][0], ptr[j][1]);
            ComplexNum result = a.complexExp();
            ptr[j][0] = result.getReal();
            ptr[j][1] = result.getImg();
        }
    }
    
}

ComplexNum toComplexNum(Mat& a) {
    return ComplexNum(a);
}