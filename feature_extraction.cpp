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
    imwrite("img/green_inverted_image.tif", channels[1]);
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
    //imwrite("img/fourier_transform.tif",magI);
    //waitKey();

    //return 0;

}

Mat getRotationMatrix(double theta) {
    Mat rot(2, 2, CV_32FC1);
    rot.ptr<double>(0)[0] = cos(theta);
    rot.ptr<double>(0)[1] = -sin(theta);
    rot.ptr<double>(1)[0] = sin(theta);
    rot.ptr<double>(1)[1] = cos(theta);
    return rot;
}

void apply2DCWT(Mat& complexI) {


    Mat A(2, 2, CV_32FC1);
    Mat rotMtrx; //rotation matrix

    //Set A
    A.ptr<double>(0)[0] = pow(elongation, -0.5);
    A.ptr<double>(0)[1] = 0;
    A.ptr<double>(1)[0] = 0;
    A.ptr<double>(1)[1] = 1;

    //Set rotation matrix
    rotMtrx = getRotationMatrix(-theta);
    //A.inv();
    Mat inv = A.inv(), morletWavelet;
    /*ComplexNum K(complexI.ptr<double>(0)[0], complexI.ptr<double>(0)[1]);

    K.printComplexNum();*/
    //morletWavelet = toComplexNum(apply2DMorletWavelet(K, inv)).conj().toMat();//Apply conjugate

    /*Mat complexExponential;
    ComplexNum complexUnit(0,1);
    
    complexExponential=((ComplexNum)(complexUnit*K)).toMat();*/

    int nrows = complexI.rows;
    int ncols = complexI.cols;
    Vec2d *ptr;

    for (int i = 500;; i++) {
        ptr = complexI.ptr<Vec2d>(i);
        for (int j = 0; j < ncols; j++) {
            ComplexNum K(ptr[j][0], ptr[j][1]);
            //K.printComplexNum();
            apply2DMorletWavelet(K, inv, morletWavelet);
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
    ComplexNum K0(0, 3), temp;
    //K0.printComplexNum();
    temp = K - K0;
    //temp.printComplexNum();
    temp = temp*temp;
    morletWavelet = temp.toMat();
    //printMatrix<double>(morletWavelet);
    //printMatrix<double>(inv);
    //printMatrix<double>(morletWavelet);
    morletWavelet = -0.5 * (inv * morletWavelet);
    //printMatrix<double>(morletWavelet);
    temp = toComplexNum(morletWavelet);
    temp = temp.complexExp();
    morletWavelet = temp.toMat() * sqrt(elongation);
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

void opticDiscSegmentation() {
    Mat image, roi;

    image = imread("image/1-background/image1.tif", 1);

    vector<Mat> channels;
    split(image, channels);


    Mat greenChannel = channels[2];
    /// Reduce the noise so we avoid false circle detection
    //GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);    
    int y = greenChannel.rows / 3;
    int x = 0;
    int w = greenChannel.cols;
    int h = y;
    roi = Mat(greenChannel, Rect(x, y, w, h));




    Scalar mean, stddev;
    meanStdDev(roi, mean, stddev);
    double _mean, _stddev;
    _mean = mean.val[0];
    _stddev = stddev.val[0];
    roi = roi - _stddev - _mean;
    int size = image.cols / 10;
    size = (size % 2 == 0) ? size + 1 : size;
    GaussianBlur(roi, roi, Size(size, size), 0, 0);
    imwrite("image/2-optic disc/image2.tif", roi);
    double max;
    minMaxLoc(roi, NULL, &max, NULL, NULL);
    threshold(roi, roi, 0.2 * max, 255, CV_THRESH_BINARY);
    //imwrite("image/2-optic disc/image2.tif", roi);

    vector<Vec3f> circles;

    HoughCircles(roi, circles, CV_HOUGH_GRADIENT, 1, 300, 15, 5, 50, 300);

    if (circles.size() == 0) {
        greenChannel = channels[1];
        /// Reduce the noise so we avoid false circle detection
        //GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);    
        y = greenChannel.rows / 3;
        x = 0;
        w = greenChannel.cols;
        h = y;
        roi = Mat(greenChannel, Rect(x, y, w, h));

        meanStdDev(roi, mean, stddev);
        _mean = mean.val[0];
        _stddev = stddev.val[0];
        roi = roi - _stddev - _mean;
        GaussianBlur(roi, roi, Size(size, size), 0, 0);
        minMaxLoc(roi, NULL, &max, NULL, NULL);
        threshold(roi, roi, 0.2 * max, 255, CV_THRESH_BINARY);
        HoughCircles(roi, circles, CV_HOUGH_GRADIENT, 1, 300, 15, 5, 50, 300);
    }
    /// Draw the circles detected
    Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    // circle center
    // circle(opticD, center, 3, Scalar(0, 255, 0), -1, 8, 0);
    // circle outline
    // circle(opticD, center, radius, Scalar(0, 0, 255), 3, 8, 0);



    Mat opticDMask = Mat(image.rows, image.cols, CV_8UC1, Scalar(255));
    //fixed center of new optic disk mask
    center.x = center.x;
    center.y = center.y + (image.rows / 3);

    circle(opticDMask, center, radius + image.rows / 40, Scalar(0), -1, 8, 0);


    Mat result;
    image.copyTo(result, opticDMask);
    /// Show your results
    imwrite("image/2-optic disc/image1.tif", result);
    /*namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", result);*/

}

void darkLessionSegmentation() {
    Mat image,mask;
    image = imread("image/2-optic disc/image1.tif", 1);

    vector<Mat> channels;

    split(image, channels);

    Mat greenChannel = channels[1];   
    //cvtColor(image, greenChannel, CV_BGR2GRAY);
    //medianBlur(greenChannel,greenChannel,3);
    Mat topHat(greenChannel.rows, greenChannel.cols, greenChannel.type()), bottomHat(greenChannel.rows, greenChannel.cols, greenChannel.type()), contrastE;
    //Mask retrieval
    greenChannel.copyTo(mask);
    //split(brightL, channels2);
    //brightL = channels2[1];
    /*Ptr<CLAHE> ptr=createCLAHE();
    ptr->apply(invG,invG);*/
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    medianBlur(mask, mask, 7);
    erode(mask, mask, element);
    threshold(mask, mask, 5, 255, CV_THRESH_BINARY);
    //imwrite("image/3-dark lession/image4.tif", mask);
    
    //greenChannel.copyTo(topHat);
    //greenChannel.copyTo(bottomHat);
    imwrite("image/3-dark lession/image4.tif", greenChannel); 
    /****TOP HAT**/
    //opening    
    element = getStructuringElement(MORPH_RECT, Size(5,5));
    /*Mat opened;
    erode(greenChannel, opened, element);
    dilate(opened, opened, element);
    //apply top hat
    topHat = topHat - opened;
    /****BOTTOM HAT**/
    //opening
    //    Mat element = getStructuringElement( MORPH_RECT,Size( 5, 5 )); 
    /*Mat closed;
    dilate(greenChannel, closed, element);
    erode(closed, closed, element);
    //apply bottom hat
    bottomHat = closed - bottomHat;*/    
    morphologyEx(greenChannel,topHat,CV_MOP_TOPHAT,element);    
    morphologyEx(greenChannel,bottomHat,CV_MOP_BLACKHAT,element);   
    contrastE = (greenChannel + topHat) - bottomHat;    
    
    Mat medianFilter;    
    medianBlur(contrastE, medianFilter, 51);
    contrastE = medianFilter - contrastE;    
    medianBlur(contrastE,contrastE,5);    
    double h = calculateMedian(contrastE,mask);
    cout<<h<<endl;
    contrastE = contrastE - h;
    Mat rec, temp;
    contrastE.copyTo(rec);
    imwrite("image/3-dark lession/image3.tif", contrastE);
    
    //do{
        rec.copyTo(temp);
        dilate(rec,rec,element);    
        bitwise_and(contrastE,rec,rec,mask); 
    //}while(); 
    
    imwrite("image/3-dark lession/image3.tif", rec);
    
    
    threshold(contrastE, contrastE, 0, 255, CV_THRESH_OTSU);
    threshold(contrastE, contrastE, 100, 255, CV_THRESH_BINARY);
    imwrite("image/3-dark lession/image1.tif", contrastE);    
}

double calculateHThreshold(Mat &image) {

    double mean = calculateMean(image);
    int nrows = image.rows;
    int ncols = image.cols;
    int sum = 0;
    double tmp;
    uchar *p;

    int counter = 0;
    for (int i = 0; i < nrows; i++) {

        p = image.ptr<uchar>(i);

        for (int j = 0; j < ncols; j++) {
            if (p[j] == 0) continue;
            tmp = p[j] - mean;
            tmp = tmp*tmp;
            sum += tmp;
            counter++;
        }
    }

    return mean - sqrt(sum / counter);
}

double calculateMean(Mat& image) {
    int nrows = image.rows;
    int ncols = image.cols;
    int sum = 0;
    int counter = 0;
    uchar *p;
    for (int i = 0; i < nrows; i++) {

        p = image.ptr<uchar>(i);

        for (int j = 0; j < ncols; j++) {
            if (p[j] == 0) continue;
            sum += p[j];
            counter++;
        }
    }

    return sum / counter;
}

int calculateMedian(Mat& image,Mat& mask) {
    /*float range[] = {0, 256};
    const float* histRange = {range};

    bool uniform = true;
    bool accumulate = false;

    Mat g_hist;
    int histSize = 256;

    unsigned int sum = 0;
    /// Compute the histograms:
    calcHist(&image, 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    int n = (image.cols * image.rows - g_hist.at<float>(0))*3 / 4;
    for (int i = 1; i < histSize; i++) {
        sum += g_hist.at<float>(i);
        //cout<<g_hist.at<float>(i)<<endl;
        if (sum >= n) {
            return i;
        }
    }
    return 255;*/
    
    Scalar s=mean(image,mask);
    double m=s.val[0];
    cout<<m<<endl;
    int nrows=image.rows;
    int ncols=image.cols;
    uchar *p,*q;
    double totalAcc=0;
    double total;
    int maskAcc=0;
    for(int i=0;i<nrows;i++){
        p=image.ptr<uchar>(i);
        q=mask.ptr<uchar>(i); 
        for(int j=0;j<ncols;j++){
            if(q[j]==0) {
                maskAcc++;
                continue;
            }
            total=p[j]-m;
            total = total*total;
            totalAcc+=total;           
        }
    }
    cout<<maskAcc<<" "<<nrows*ncols<<endl;
    return (int)(sqrt(totalAcc))*(2/((nrows*ncols)-maskAcc));
    
}

void vesselSegmentation() {
    int lineOperator = 15;
    int step = 15;
    Mat image, invG, brightL;
    image = imread("image/2-optic disc/image1.tif", 1);
    //brightL = imread("image/5-bright lesion/image1.tif", 1);

    vector<Mat> channels;
    split(image, channels);
    Mat mask;
    Mat tmp2;
    invG = channels[1];
    invG.copyTo(mask);
    //split(brightL, channels2);
    //brightL = channels2[1];
    /*Ptr<CLAHE> ptr=createCLAHE();
    ptr->apply(invG,invG);*/
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    medianBlur(mask, mask, 7);
    erode(mask, mask, element);
    
         
    //bitwise_not(brightL, mask, mask);    
    bitwise_not(invG, invG, mask);

    //element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    //Pre- processing

    Mat median,topHat, opened;
    /*medianBlur(invG, invG, 3);
    invG.copyTo(tmp2);
    medianBlur(invG, median, 75);
    invG = invG - median;
    erode(tmp2, opened, element);
    dilate(opened, opened, element);
    //apply top hat    
    topHat = tmp2 - opened;   
    //threshold(topHat,topHat,0.01*255,255,CV_THRESH_BINARY);
    invG = invG - topHat;
    //threshold(invG,invG,0.02*255,255,CV_THRESH_TOZERO);
    /*namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", invG);*/
    
    //equalizeHist(invG,invG);
    invG.copyTo(tmp2);   
    erode(tmp2, opened, element);
    dilate(opened, opened, element);
    topHat = tmp2 - opened;
    invG = invG - topHat;  
    
    medianBlur(invG,invG,3);
    medianBlur(invG, median, 105);
    invG = invG - median;
    //double max;
    //minMaxLoc(invG,NULL,&max,NULL,NULL,mask);
    //threshold(invG,invG,0.1*max,255,CV_THRESH_TOZERO);    
    
    imwrite("image/4-vessel/image2.tif", invG);
    
    

    int nrows = invG.rows - lineOperator;
    int ncols = invG.cols - lineOperator;
    uchar *p, *q, *r;

    vector<vector<Point> > lineIt = calculateLineIterators(lineOperator, step); //[180 / step - 1];
    vector<vector<Point> > ortIt = calculateOrtLineIterators(lineOperator, step);
    Mat tmp = Mat(invG.rows, invG.cols, invG.type()), square;
    //vector<Point> *line, *ort;
    //double mainStr, weighted, ortStr;
    for (int i = 0; i < nrows; i++) {
        //p = invG.ptr<uchar>(i);
        q = mask.ptr<uchar>(i);
        r = tmp.ptr<uchar>(i);
        for (int j = 0; j < ncols; j++) {
            if (q[j] == 0) continue;
            square = Mat(invG, Rect(j, i, lineOperator, lineOperator));
            //if (getLineResponse(square, lineIt, ortIt) > 2.5) {
            if (getLineResponse(square, lineIt, ortIt)) {
                r[j] = 255;
            }
            //cout<<ortStr<<endl;            
        }
    }
    //Mat e = getStructuringElement(MORPH_RECT, Size(3, 3));

    //dilate(tmp,tmp,element);
    //connectedComponents(tmp,tmp);    
    
    
    imwrite("image/4-vessel/image1.tif", tmp);
    /*namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", tmp);*/
}

double getLineResponse(Mat &square, vector<vector<Point> > &lineIt, vector<vector<Point> > &ortIt) {
    vector<Point> *line, *ort;
    double max = -100000, mainStr, ortStr, min = 999999;
    int size = lineIt.size();
    int f;
    for (int it = 0; it < size; it++) {
        line = &lineIt[it];
        ort = &ortIt[it];
        mainStr = calculateLineStrength(square, *line);
        
        if(mainStr >5){            
            ortStr = calculateLineStrength(square, *ort);
            f=mainStr+ortStr;
        }else{
            f=mainStr;
        }
        
        //calculate max line operator response
        if (f > max) {
            max = f;            
        }
        if (f < min) {
            min = f;
        }
    }
    //return 0.4 * ortStr + 0.6 * max;
    int t = 0.25 * (12 - min);
    if (max>t)
        return max;
    else
        return 0;

}

void getLinePoints(int l, Point &start, Point &end, double theta) {

    Point center(l / 2, l / 2);
    int flag = 0;
    //convert to radians
    if (theta != 0 && theta != 90) {
        if (theta > 90)flag = 1;
        theta = 3.1416 / 180 * theta;
        double m = abs(tan(theta));


        if (m <= 1) {
            end.x = l / 2;
            end.y = m * end.x;
            start.x = -l / 2;
            start.y = m * start.x;
        } else {
            end.y = l / 2;
            end.x = end.y / m;
            start.y = -l / 2;
            start.x = start.y / m;
        }
        if (flag) {
            end.x *= -1;
            start.x *= -1;
        }
        end.x += center.x;
        end.y += center.y;
        start.x += center.x;
        start.y += center.y;
    } else {//tan theta inderterminated
        if (theta == 0) {
            end.x = center.x + l / 2;
            end.y = center.y;
            start.x = center.x - l / 2;
            start.y = center.y;
        } else {
            end.y = center.y + l / 2;
            end.x = center.x;
            start.y = center.y - l / 2;
            start.x = center.x;
        }
    }
}

void getOrtogonalLinePoints(int l, Point &start, Point &end, double theta) {
    int ortP = 3;
    int pix = ortP / 2;
    Point center(l / 2, l / 2);
    if (theta == 0 || theta == 180) {
        start.x = center.x;
        start.y = center.y - pix;
        end.x = center.x;
        end.y = center.y + pix;
    } else if (theta > 0 && theta < 90) {
        start.x = center.x + pix;
        start.y = center.y - pix;
        end.x = center.x - pix;
        end.y = center.y + pix;
    } else if (theta > 90 && theta < 180) {
        start.x = center.x + pix;
        start.y = center.y + pix;
        end.x = center.x - pix;
        end.y = center.y - pix;
    }


}

double calculateLineStrength(Mat &img, vector<Point> &it) {
    Scalar m = sum(img);
    //double mean = m.val[0]/(img.rows*img.cols-it.count);
    double lineMean = 0;
    Point a(0, 0);
    uchar *p;
    int size = it.size();
    for (int i = 0; i < size; i++) {
        a = it[i];
        p = img.ptr<uchar>(a.y);
        lineMean += p[a.x];
        //lineMean+=img.at<uchar>(a);         
        //it++;
    }
    double mean = (m.val[0] - lineMean) / (img.rows * img.cols - size);
    lineMean = lineMean / size;
    
    return lineMean - mean;    
}

vector<vector<Point> > calculateLineIterators(int l, int step) {
    vector<vector<Point> > iterators;
    Point start(0, 0), end(0, 0);
    for (int t = 0; t < 180; t = t + step) {//t means theta (angle)                    
        getLinePoints(l, start, end, t);
        //LineIterator mainLine(m, start, end, 8, true);        
        iterators.push_back(getBSHLine(start, end));
    }
    return iterators;
}

vector<vector<Point> > calculateOrtLineIterators(int l, int step) {
    vector<vector<Point> > iterators;
    Point startO(0, 0), endO(0, 0);
    for (int t = 0; t < 180; t = t + step) {//t means theta (angle)                    
        getOrtogonalLinePoints(l, startO, endO, t);
        //LineIterator ortogonalLine(m, startO, endO, 8, true);        
        iterators.push_back(getBSHLine(startO, endO));
    }
    return iterators;
}

vector<Point> getBSHLine(Point &start, Point &end) {
    vector<Point> line;
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float error = 0;
    float deltaerr = abs(dy / dx);
    int y = start.y;
    int x = start.x;
    int ysign = dy >= 0 ? 1 : -1;
    int xsign = dx >= 0 ? 1 : -1;
    if (dx == 0) {
        for (int i = start.y; i <= end.y; i++) {
            line.push_back(Point(x, i));
        }
        return line;
    }

    while (true) {
        line.push_back(Point(x, y));
        //cout<<Point(x,y)<<endl;
        if (x == end.x) {
            break;
        }
        error += deltaerr;
        while (error >= 0.5) {
            y += ysign;
            error--;
            //cout<<Point(x,y)<<endl;
            line.push_back(Point(x, y));
        }
        x += xsign;
    };
    return line;
}

void brightLessionSegmentation() {
    Mat image, mask;
    image = imread("image/2-optic disc/image1.tif", 1);


    //bitwise_not(image, image, mask);

    vector<Mat> channels;
    split(image, channels);
    Mat gc = channels[1], lab;
    Mat greenChannel;
    gc.copyTo(greenChannel);
    greenChannel.copyTo(mask);
    /*Ptr<CLAHE> ptr=createCLAHE();
    ptr->apply(invG,invG);*/
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    medianBlur(mask, mask, 5);
    erode(mask, mask, element);
    threshold(mask, mask, 5, 255, CV_THRESH_BINARY);

    Mat l_chann;
    cvtColor(image, lab, CV_BGR2Lab);
    split(lab, channels);
    l_chann = channels[0]; //assign L channel


    //Thresholding method
    element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
    Mat topHat, bottomHat;
    l_chann.copyTo(topHat);
    l_chann.copyTo(bottomHat);
    //opening      
    Mat opened;
    erode(l_chann, opened, element);
    dilate(opened, opened, element);
    //apply top hat
    topHat = topHat - opened;
    /****BOTTOM HAT**/
    //opening
    //    Mat element = getStructuringElement( MORPH_RECT,Size( 5, 5 )); 
    Mat closed;
    dilate(l_chann, closed, element);
    erode(closed, closed, element);
    //apply bottom hat
    bottomHat = closed - bottomHat;
    l_chann = l_chann + topHat - bottomHat;

    //normalize(l_chann, l_chann, 0, 255, CV_MINMAX);

    /*Ptr<CLAHE> ptr = createCLAHE();    
    ptr->apply(l_chann, l_chann);*/
    equalizeHist(l_chann, l_chann);
    double min, max;
    minMaxLoc(l_chann, &min, &max);

    double t = max - 0.01 * max;
    threshold(l_chann, l_chann, t, 255, CV_THRESH_BINARY);
    imwrite("image/5-bright lesion/image3.tif", l_chann);
    //Morphological method
    Mat i1, i2;
    element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
    dilate(greenChannel, i1, element);
    element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    dilate(greenChannel, i2, element);

    greenChannel = i1 - i2;
    threshold(greenChannel, greenChannel, 0.04 * 255, 255, CV_THRESH_BINARY);
    imwrite("image/5-bright lesion/image2.tif", greenChannel);


    //Complementation of previous techniques
    Mat c1, c2;
    element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    dilate(greenChannel, c1, element);
    erode(c1, c1, element);

    element = getStructuringElement(MORPH_ELLIPSE, Size(23, 23));
    dilate(greenChannel, c2, element);
    erode(c2, c2, element);

    bitwise_and(c1, l_chann, c1, mask);
    bitwise_and(c2, l_chann, c2, mask);


    Mat i5, i6;
    bitwise_xor(c1, c2, i5, mask);
    bitwise_and(c1, c2, i6, mask);

    Mat finalOutput;

    finalOutput = i5 + i6;

    imwrite("image/5-bright lesion/image1.tif", finalOutput);
    // imwrite("image/5-bright lession/image2.tif",greenChannel);
    /*namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", l_chann);*/



}

void iluminationEqualization(Mat& input, Mat& output, Mat& mask) {
    Mat out(input.rows, input.cols, input.type());
    int kernel_size = 3;
    int intensity = 20;
    int nrows = input.rows - kernel_size;
    int ncols = input.cols - kernel_size;
    uchar *p, *q, *r;
    Mat kernel;
    for (int i = 0; i < nrows; i++) {
        p = input.ptr<uchar>(i);
        q = out.ptr<uchar>(i);
        r = mask.ptr<uchar>(i);
        for (int j = 0; j < ncols; j++) {
            if (r[j] == 0) continue;
            kernel = Mat(input, Rect(j, i, kernel_size, kernel_size));
            Scalar _mean = mean(kernel);
            double m = _mean.val[0];
            q[j] = p[j] + intensity - m;
        }
    }
    output = out;
}
