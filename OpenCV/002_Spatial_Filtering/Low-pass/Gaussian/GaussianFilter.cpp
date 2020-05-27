//============================================================================
// Name        : GaussianFilter.cpp
// Author      : Seungbin Ahn
// Version     : 1.0
// Description : Gaussian Smoothing Filter
//============================================================================

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

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

void main() {
	Mat src = imread("Lena.jpg", 1);
	Mat dst;
	//Apply Gaussian filter

	// ksize : 필터 크기
	// sigma : 시그마
	// ktype : CV_32F or CV_64F(Default) 
	Mat filter = getGaussianKernel(13, 2.0, CV_64F);
	cout << filter << "\n";

	// src : 원본
	// dst : output
	// ksiz : Gaussian kernel size
	// sigmaX
	// sigmaY
	GaussianBlur(src, dst, Size(13, 13), 0, 0);
	imshow("source", src);
	imshow("result", dst);
	waitKey(0);
}

