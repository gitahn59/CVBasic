//============================================================================
// Name        : main.cpp
// Author      : gitahn59
// Version     : 1.0
// Copyright   : MIT
// Description : prewise-linear transformation
//============================================================================

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

/**
* Calculate prewise-linear transform value
* 
* @param x : source value
* @param r1 : boundary value 
* @param s1 : boundary value 
* @param r2 : boundary value 
* @param s2 : boundary value
*/
uchar computeOutput(uchar x, int r1, int s1, int r2, int s2);

/**
* Make window to show image
*
* @param name : title of Window
* @param size : size of Window
* @param img : image to show
*/
void showMyWindow(string name, Size size, const Mat& img) {
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, img);
	resizeWindow(name, size);
}

int main() {
	Mat src = imread("washed_out_pollen_image.tif", 0); // src image
	Mat result = src.clone(); // prewise-linear transformation image
	Mat bin; // thresholde image
	int s;
	int r1, r2, s1, s2;

	r1 = 70;
	r2 = 140;
	s1 = 50;
	s2 = 220;

	//prewise-linear transformation
	for (int x = 0; x < src.rows; x++) {
		for (int y = 0; y < src.cols; y++) {
			s = computeOutput(src.at<uchar>(x, y), r1, s1, r2, s2);
			result.at<uchar>(x, y) = saturate_cast<uchar>(s);
		}
	}

	//threshold
	threshold(result, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// show images
	Size size(450, 450);
	showMyWindow("src", size, src);
	showMyWindow("result", size, result);
	showMyWindow("threshold", size, bin);
	waitKey(0);
	return 0;
}

uchar computeOutput(uchar x, int r1, int s1, int r2, int s2) {
	uchar result = 0;
	double inclination = 0; // inclination of straight line
	if (0 <= x && x <= r1) { 
		double inclination = ((double)s1 / (double)r1);
		result = inclination * x ; 
	}
	else if (r1 < x && x <= r2) {
		double inclination = (double)(s2 - s1) / (double)(r2 - r1);
		result = inclination * (x - r1) + s1; 
	else {
		double inclination = (double)(255 - s2) / (double)(255 - r2);
		result = inclination * (x - r2) + s2; 
	}

	return result;
}