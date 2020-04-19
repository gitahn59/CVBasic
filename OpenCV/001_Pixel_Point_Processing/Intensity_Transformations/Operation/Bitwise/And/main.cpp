//============================================================================
// Name        : main.cpp
// Author      : gitahn59
// Version     : 1.0
// Copyright   : MIT
// Description : Bitwise And Operation
//============================================================================

#include <opencv2/opencv.hpp>
#include <string>

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

int main() {
	Mat src = imread("dental_xray.tif", IMREAD_GRAYSCALE);
	Mat mask = imread(dental_xray_mask.tif", IMREAD_GRAYSCALE);
	CV_Assert(!(src.empty() || mask.empty()));

	Mat result;
	//bitwise and operation
	bitwise_and(src, mask, result);

	// show images
	Size size(300, 200);
	showMyWindow("src", size, src);
	showMyWindow("mask", size, mask);
	showMyWindow("result", size, result);

	waitKey(0);
}
