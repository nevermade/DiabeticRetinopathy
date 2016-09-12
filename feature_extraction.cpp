/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cstdlib>
#include "feature_extraction.h"

using namespace cv;
using namespace std;

double step = 0.2618, max = 2.8798, dilation = 3, elongation = 4, theta = 0;


void opticDiscSegmentation(Mat& bgMask, Mat& image) {
    Mat roi, median;

    //image = imread("image/1-background/image1.tif", 1);
    //readInGreenChannel("image/1-background/mask1.tif", bgMask);
    vector<Mat> channels;
    split(image, channels);


    //Mat greenChannel = channels[2];
    Mat greenChannel;
    cvtColor( image, greenChannel, CV_BGR2GRAY );
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
    //imwrite("image/2-optic disc/image2.tif", roi);
    double max;
    minMaxLoc(roi, NULL, &max, NULL, NULL);
    threshold(roi, roi, 0.3 * max, 255, CV_THRESH_BINARY);
    //imwrite("image/2-optic disc/image2.tif", roi);

    vector<Vec3f> circles;

    HoughCircles(roi, circles, CV_HOUGH_GRADIENT, 1, image.cols, 30, 10, 0, 0);
    //HoughCircles(roi, circles, CV_HOUGH_GRADIENT, 1, image.cols/2, 100, 50, 0, 0);

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
        threshold(roi, roi, 0.3 * max, 255, CV_THRESH_BINARY);
        HoughCircles(roi, circles, CV_HOUGH_GRADIENT, 1, image.cols, 30, 10, 0, 0);
        //HoughCircles(roi, circles, CV_HOUGH_GRADIENT, 1, image.cols/2, 100, 50, 0, 0);
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

    circle(opticDMask, center, radius + 50, Scalar(0), -1, 8, 0);

    
    //Mat result;
    
    /// Show your results
    Mat finalImage;
    bitwise_and(bgMask, opticDMask, bgMask);
    image.copyTo(finalImage, bgMask);
    image=finalImage;
    //imwrite("image/2-optic disc/image1.tif", result);
    //imwrite("image/3-final mask/mask1.tif", bgMask);
    /*namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", result);*/

}

void darkLessionSegmentation(Mat& bgMask, Mat& image, CharsImage& ci) {



    Mat invG;
    readInGreenChannel(image, invG);
    //readInGreenChannel("image/3-final mask/mask1.tif", bgMask);
    
    
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));


    /*green_image.copyTo(bgMask);
    medianBlur(bgMask, bgMask, 7);
    erode(bgMask, bgMask, element);*/
    // apply the CLAHE algorithm to the L channel


    bitwise_not(invG, invG, bgMask);
    Mat medianFilter, topHat;
    //medianBlur(invG, invG, 3);
    Ptr<CLAHE> ptr = createCLAHE();
    ptr->setClipLimit(4);
    ptr->apply(invG, invG);
    medianBlur(invG, medianFilter, 105);
    invG = invG - medianFilter;
    morphologyEx(invG, topHat, CV_MOP_TOPHAT, element);
    invG = invG - topHat;    
    GaussianBlur(invG, invG, Size(7, 7),0,0);
    //GaussianBlur(invG, invG, Size(9, 9), 0, 0);
    //imwrite("image/5-vessel/image3.tif", invG);
    //imwrite("image/5-vessel/image4.tif", topHat);
    double max, min;
    minMaxLoc(invG, &min, &max, NULL, NULL, bgMask);
    threshold(invG, invG, 0.25 * max, 255, CV_THRESH_BINARY);
    //imwrite("image/4-dark lession/invg.tif", invG);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //Canny(invG, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
    findContours(invG, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE);


    //Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC1);


    Mat maImage = Mat::zeros(invG.rows, invG.cols, invG.type());
    double area;
    Rect r;
    vector<vector<Point> > filteredContours;
    double k;
    for (int i = 0; i < contours.size(); i++) {
        area = contourArea(contours.at(i));
        r = boundingRect(contours.at(i));
        if(r.height>r.width){
            max=r.height;
            min=r.width;
        }else{
            max=r.width;
            min=r.height;
        }
        
        k = min/max;
        if ((k >= 0.8) && area <= 800 && area >= 0.55 * r.height * r.width && area>=20) filteredContours.push_back(contours.at(i));
        //if(area>=500) filteredContours.push_back(contours.at(i));

    }
    for (int i = 0; i < filteredContours.size(); i++) {
        drawContours(maImage, filteredContours, i, Scalar(255), CV_FILLED, 8, hierarchy, 0, Point());
    }
    
    
    //dilate(maImage,maImage,element);
    ci.areaDarkZone=countNonZero(maImage);
    ci.numberDarkZone=filteredContours.size();

    imwrite("image/4-dark lession/image"+SSTR(ci.nImage)+".tif", maImage);
    
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

int calculateMedian(Mat& image, Mat& mask) {
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

    Scalar s = mean(image, mask);
    double m = s.val[0];
    cout << m << endl;
    int nrows = image.rows;
    int ncols = image.cols;
    uchar *p, *q;
    double totalAcc = 0;
    double total;
    int maskAcc = 0;
    for (int i = 0; i < nrows; i++) {
        p = image.ptr<uchar>(i);
        q = mask.ptr<uchar>(i);
        for (int j = 0; j < ncols; j++) {
            if (q[j] == 0) {
                maskAcc++;
                continue;
            }
            total = p[j] - m;
            total = total*total;
            totalAcc += total;
        }
    }
    cout << maskAcc << " " << nrows * ncols << endl;
    return (int) (sqrt(totalAcc))*(2 / ((nrows * ncols) - maskAcc));

}

void getLineImageResponse(Mat& input, Mat& output, Mat& mask, int lineLength, int lineStep) {

    int nrows = input.rows - lineLength;
    int ncols = input.cols - lineLength;
    int offset = lineLength / 2;
    uchar *q, *r;


    vector<vector<Point> > lineIt = calculateLineIterators(lineLength, lineStep); //[180 / lineStep - 1];
    Point ortIt[12][3];
    calculateOrtLineIterators(ortIt, lineLength, lineStep);
    //vector<vector<Point> > ortIt = calculateOrtLineIterators(ortIt, lineLength, lineStep);
    output = Mat::zeros(input.rows, input.cols, input.type());
    Mat square;

    //vector<Point> *line, *ort;
    //double mainStr, weighted, ortStr;
    int threshold;
    if(lineLength==15)
        threshold=6;
    else
        threshold=12;
    
    
    for (int i = 0; i < nrows; i++) {
        //p = input.ptr<uchar>(i);
        q = input.ptr<uchar>(i + offset);
        r = output.ptr<uchar>(i + offset);
        for (int j = 0; j < ncols; j++) {
            if (q[j + offset] <= 5) continue;
            square = Mat(input, Rect(j, i, lineLength, lineLength));
            //if (getLineResponse(square, lineIt, ortIt) > 2.5) {
            if (getLineResponse(square, lineIt, ortIt)>threshold) {
                r[j + offset] = 255;
            }
            //cout<<ortStr<<endl;       

        }

    }

    /*    
    Mat detected_edges;
    int ratio = 3;
    int kernel_size = 3;
    int lowThreshold = 35;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Canny(tmp, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
    findContours(detected_edges, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE);


    //Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC1);


    Mat maImage = Mat::zeros(tmp.rows, tmp.cols, tmp.type());
    double area;
    Rect re;
    vector<vector<Point> > filteredContours;
    double k;
    for (int i = 0; i < contours.size(); i++) {
        area = contourArea(contours.at(i));
        re = boundingRect(contours.at(i));
        k = re.height / re.width;
        if ((k >= 0.7 && k <= 1.3) && area <= 800 && area >= 0.2*re.height*re.width ) filteredContours.push_back(contours.at(i));
        //if(area>=500) filteredContours.push_back(contours.at(i));

    }
    for (int i = 0; i < filteredContours.size(); i++) {
        drawContours(maImage, filteredContours, i, Scalar(255), CV_FILLED, 8, hierarchy, 0, Point());
    }
    
    output=tmp-maImage; */
    //Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    //morphologyEx(tmp, output, CV_MOP_OPEN, element);



    //imwrite("image/5-vessel/image2.tif", maImage);
    //imwrite("image/5-vessel/image1.tif", tmp);

}

void vesselSegmentation(Mat& bgMask, Mat& maImage) {

    Mat invG;
    int step = 15;

    readInGreenChannel("image/2-optic disc/image1.tif", invG);
    readInGreenChannel("image/3-final mask/mask1.tif", bgMask);
    readInGreenChannel("image/4-dark lession/maimage1.tif", maImage);


    //brightL = imread("image/5-bright lesion/image1.tif", 1);
    //split(brightL, channels2);
    //brightL = channels2[1];
    /*Ptr<CLAHE> ptr=createCLAHE();
    ptr->apply(invG,invG);*/
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));


    //bitwise_not(brightL, mask, mask);    
    bitwise_not(invG, invG, bgMask);

    //element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    //Pre- processing   

    Mat medianFilter, topHat;
    medianBlur(invG, invG, 3);
    Ptr<CLAHE> ptr = createCLAHE();
    ptr->setClipLimit(4);
    ptr->apply(invG, invG);
    medianBlur(invG, medianFilter, 105);
    invG = invG - medianFilter;
    morphologyEx(invG, topHat, CV_MOP_TOPHAT, element);
    invG = invG - topHat;
    GaussianBlur(invG, invG, Size(9, 9), 3.25, 3.25);
    //imwrite("image/5-vessel/image3.tif", invG);
    //imwrite("image/5-vessel/image4.tif", topHat);
    /*double max, min;
    minMaxLoc(invG, &min, &max, NULL, NULL, bgMask);
    threshold(invG, invG, 0.25 * max, 255, CV_THRESH_BINARY);*/


    //invG = invG - maImage;
    //imwrite("image/5-vessel/image3.tif", invG);

    Mat imR15, imR30, result;

    //getLineImageResponse(invG, imR15, bgMask, 15, step);
    //imwrite("image/5-vessel/im15.tif", imR15);
    getLineImageResponse(invG, imR30, bgMask, 30, step);
    imwrite("image/5-vessel/im30.tif", imR30);
    //result = imR15 - imR30;
    //morphologyEx(result, result, CV_MOP_OPEN, element);
    //imwrite("image/5-vessel/final.tif", result);




}

void readBinaryInBinary(String path, Mat& binary) {
    Mat image;

    image = imread(path, 1);
    vector<Mat> channels;
    split(image, channels);
    binary = channels[1];

}

void readInGreenChannel(const String& path, Mat& image) {
    Mat im;
    im = imread(path, 1);
    vector<Mat> channels;
    split(im, channels);
    image = channels[1];
}

void readInGreenChannel(Mat& src, Mat& image) {
    
    vector<Mat> channels;
    split(src, channels);
    image = channels[1];
}

double getLineResponse(Mat &square, vector<vector<Point> > &lineIt, Point ortIt[][3]) {
    //vector<Point> *line, *ort;
    double max = -100000, mainStr, ortStr, min = 999999;
    int size = lineIt.size();
    int f;
    for (int it = 0; it < size; it++) {
        //line = &lineIt[it];
        //ort = &ortIt[it];

        mainStr = calculateLineStrength(square, lineIt[it]);
        //ortStr = calculateLineStrength(square, ortIt[it]);
        //calculate max line operator response
        
        if (mainStr > 3) {
            ortStr = calculateLineStrength(square, ortIt[it]);
            if(ortStr>2){
                f = mainStr;
            }else
                f=0;
            
        } else
            f = 0;

        if (f > max) {
            max = f;
        }
        if (f < min) {
            min = f;
        }
    }

    if (max)
        return max;
    else
        return 0;

}

void getLinePoints(int l, Point &start, Point &end, double theta) {

    Point center(l / 2, l / 2);
    int flag = 0;
    //convert to radians
    if (theta != 0 && theta != 90) {
        if (theta > 90) {
            flag = 1;
            theta = 180 - theta;
        }
        theta = 3.1416 / 180 * theta;
        double m = tan(theta);


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
    if (theta == 0) {
        start.x = center.x;
        start.y = center.y - pix;
        end.x = center.x;
        end.y = center.y + pix;
    } else if (theta == 1) {
        start.x = center.x + pix;
        start.y = center.y - pix;
        end.x = center.x - pix;
        end.y = center.y + pix;
    } else if (theta == 2) {
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
    //double mean = (m.val[0] - lineMean) / (img.rows * img.cols - size);
    lineMean = lineMean / size;

    return lineMean - mean;
}

double calculateLineStrength(Mat &img, Point it[3]) {
    Scalar m = sum(img);
    //double mean = m.val[0]/(img.rows*img.cols-it.count);

    double lineMean = 0;
    Point a(0, 0);
    uchar *p;
    int size = 3;

    for (int i = 0; i < size; i++) {
        a = it[i];
        p = img.ptr<uchar>(a.y);
        lineMean += p[a.x];
        //lineMean+=img.at<uchar>(a);         
        //it++;
    }
    double mean = (m.val[0] - lineMean) / (img.rows * img.cols - size);
    //double mean = (m.val[0] - lineMean) / (img.rows * img.cols - size);
    lineMean = lineMean / size;

    return lineMean - mean;
}

vector<vector<Point> > calculateLineIterators(int l, int step) {
    vector<vector<Point> > iterators;
    Point start(0, 0), end(0, 0);
    for (int t = 0; t < 180; t = t + step) {//t means theta (angle)                    
        getLinePoints(l, start, end, t);
        //LineIterator mainLine(m, start, end, 8, true);        
        iterators.push_back(getDDALine(start, end));

    }
    return iterators;
}

void calculateOrtLineIterators(Point it[12][3], int l, int step) {
    //vector<vector<Point> > iterators;
    Point startO(0, 0), endO(0, 0), mid(l / 2, l / 2);

    for (int t = 0; t < 12; t++) {
        getOrtogonalLinePoints(l, startO, endO, t);
        //LineIterator ortogonalLine(m, startO, endO, 8, true);        
        //iterators.push_back(getBSHLine(startO, endO));
        it[t][0] = startO;
        it[t][1] = mid;
        it[t][2] = endO;
    }
    //return iterators;
}

vector<Point> getDDALine(Point &start, Point &end) {
    vector<Point> line;
    int length;
    float dx = abs(end.x - start.x);
    float dy = abs(end.y - start.y);
    float error = 0;
    float deltaerr = abs(dy / dx);
    if (dx >= dy)
        length = dx;
    else
        length = dy;
    dx=dx/length;
    dy=dy/length;
    dx = (end.x - start.x) >= 0 ? dx : -1*dx;
    dy = (end.y - start.y) >= 0 ? dy : -1*dy;    
    float y = start.y;
    float x = start.x;
    
    for(int i=0;i<=length;i++){
        line.push_back(Point(round(x),round(y)));
        x+=dx;
        y+=dy;        
    }
    /*int ysign = dy >= 0 ? 1 : -1;
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
    };*/
    return line;
}

void brightLessionSegmentation(Mat& mask, Mat& image, CharsImage& ci) {
    
    //image = imread("image/2-optic disc/image1.tif", 1);


    //bitwise_not(image, image, mask);

    vector<Mat> channels;
    split(image, channels);
    Mat gc = channels[1];
    Mat greenChannel;
    gc.copyTo(greenChannel);
    /*greenChannel.copyTo(mask);
    /*Ptr<CLAHE> ptr=createCLAHE();
    ptr->apply(invG,invG);
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    medianBlur(mask, mask, 5);
    erode(mask, mask, element);
    threshold(mask, mask, 5, 255, CV_THRESH_BINARY);*/
    
    Mat lab;
    Mat l_chann;
    
    cvtColor(image, lab, CV_BGR2Lab);
    split(lab, channels);
    l_chann = channels[0]; //assign L channel


    //Thresholding method
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
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
    //imwrite("image/5-bright lesion/image3.tif", l_chann);
    //Morphological method
    Mat i1, i2;
    element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
    dilate(greenChannel, i1, element);
    element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    dilate(greenChannel, i2, element);

    greenChannel = i1 - i2;
    threshold(greenChannel, greenChannel, 0.04 * 255, 255, CV_THRESH_BINARY);
    //imwrite("image/5-bright lesion/image2.tif", greenChannel);


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

    element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    erode(finalOutput,finalOutput,element);
    dilate(finalOutput, finalOutput, element);
    
    //imwrite("image/6-bright lesion/image1.tif", finalOutput);
      
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //Canny(finalOutput, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
   
    //findContours(finalOutput, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(finalOutput, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE);

    vector<vector<Point> > filteredContours;
    double k,area;
    
    Rect r;
    for (int i = 0; i < contours.size(); i++) {
        area = contourArea(contours.at(i));
        r = boundingRect(contours.at(i));
        if(r.height>r.width){
            max=r.height;
            min=r.width;
        }else{
            max=r.width;
            min=r.height;
        }
        
        k = min/max;
        if ((k >= 0.3) && area >=10 && area >= 0.3 * r.height * r.width) filteredContours.push_back(contours.at(i));
        //if (area>=10) filteredContours.push_back(contours.at(i));
        //if(area>=500) filteredContours.push_back(contours.at(i));

    }
    
    
    Mat maImage = Mat::zeros(finalOutput.rows, finalOutput.cols, finalOutput.type());
    for (int i = 0; i < filteredContours.size(); i++) {
        drawContours(maImage, filteredContours, i, Scalar(255), CV_FILLED, 8, hierarchy, 0, Point());
    }
    imwrite("image/6-bright lesion/image"+SSTR(ci.nImage)+".tif", maImage);
    ci.areaBrightZone=countNonZero(maImage);
    ci.numberBrightZones=filteredContours.size();
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
