//============================================================================
// Name        : main.cpp
// Author      : gitahn59
// Version     : 1.0
// Copyright   : MIT
// Description : Contrast Enhancement using Power Law Transformation
//============================================================================

// p=1 : r = s
// p<1 : like log
// p>1 : like nverse log

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

void main()
{
	Mat src = imread("Fig0309(a)(washed_out_aerial_image).tif");
	Mat fImage;
	src.convertTo(fImage, CV_32F); // conver to CV_32F to pow law transformation

	double power1 = 3.0;
	Mat powImage1;
	pow(fImage, power1, powImage1); // power law transformation
	normalize(powImage1, powImage1, 0, 1.0, NORM_MINMAX); // normalize between 0 and 1.0  

	double power2 = 4.0;
	Mat powImage2;
	pow(fImage, power2, powImage2); // power law transformation
	normalize(powImage2, powImage2, 0, 1.0, NORM_MINMAX); // normalize between 0 and 1.0

	double power3 = 5.0;
	Mat powImage3;
	pow(fImage, power3, powImage3); // power law transformation
	normalize(powImage3, powImage3, 0, 1.0, NORM_MINMAX); // normalize between 0 and 1.0

	Size size(500, 400); // window size 
	// Show Window
	showMyWindow("src", size, src);
	showMyWindow("power = 3.0", size, powImage1);
	showMyWindow("power = 4.0", size, powImage2);
	showMyWindow("power = 5.0", size, powImage3);

	waitKey(0);
}