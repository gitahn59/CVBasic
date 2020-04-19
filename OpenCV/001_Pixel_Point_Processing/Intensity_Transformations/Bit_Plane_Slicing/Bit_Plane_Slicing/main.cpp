//============================================================================
// Name        : main.cpp
// Author      : gitahn59
// Version     : 1.0
// Copyright   : MIT
// Description : Bit Plane Slicing 
//============================================================================

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

char print(int i) {
	char temp = (char)(i + '0');
	return temp;
}

int main()
{
	Mat m1 = imread("100-dollars.tif", IMREAD_GRAYSCALE); // read image

	Size size(476, 200);
	//show original image
	string orign = "Original";
	namedWindow(orign, WINDOW_NORMAL);
	imshow(orign, m1);
	resizeWindow(orign, size);

	int cols, rows, x, y;
	cols = m1.cols;
	rows = m1.rows;
	printf("%d %d \n", m1.rows, m1.cols); // print row and col

	// init empty images
	Mat out[8];
	for (int i = 0; i <= 7; i++) {
		out[i] = Mat(rows, cols, CV_8UC1, Scalar(0));
	}

	for (int i = 0; i <= 7; i++) {
		for (int x = 0; x < rows; x++) {
			for (int y = 0; y < cols; y++) {
				// if (value == 2^i) 
				out[i].at<uchar>(x, y) = (m1.at<uchar>(x, y) & uchar(pow(2, i))) ? uchar(255) : uchar(0);
			}
		}
		// show window
		string s = "dollorImg";
		s += print(i);
		namedWindow(s, WINDOW_NORMAL);
		imshow(s, out[i]);
		resizeWindow(s, size);
	}

	waitKey(0);

	return 0;
}