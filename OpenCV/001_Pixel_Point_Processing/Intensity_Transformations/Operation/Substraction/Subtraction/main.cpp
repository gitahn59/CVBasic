//============================================================================
// Name        : main.cpp
// Author      : gitahn59
// Version     : 1.0
// Copyright   : MIT
// Description : Contrast Enhancement using Power Law Transformation
//============================================================================

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
	//Read Image
	Mat image1 = imread("angiography_mask.tif", IMREAD_GRAYSCALE);
	Mat image2 = imread("angiography_live.tif", IMREAD_GRAYSCALE);
	CV_Assert(!(image1.empty() || image2.empty()));

	//substraction
	Mat sub_img = abs(image2 - image1);

	// copy image
	Mat fImage;
	sub_img.convertTo(fImage, CV_32F); 

	// Make new image to enhance
	Mat enhanced_img;
	pow(fImage, 0.5, enhanced_img); // pow raw transform
	normalize(enhanced_img, enhanced_img, 0, 1.0, NORM_MINMAX); 

	// show windows
	imshow("Mask", image1), imshow("live", image2); 
	imshow("sub_img", sub_img);
	imshow("enhanced_img", enhanced_img);

	waitKey(0);
}
