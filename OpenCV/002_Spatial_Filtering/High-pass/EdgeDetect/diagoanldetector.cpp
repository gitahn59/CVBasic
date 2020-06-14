//============================================================================
// Name        : diagonaldectector.cpp
// Author      : Seungbin Ahn
// Version     : 1.0
// Description : Diagonal Dectector
//============================================================================

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace cv;
using namespace std;

// filter 값을 저장하는 2차원 vector
typedef vector<vector<double>> vvf;

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

/**
* mask를 이용하여 필터를 생성한다.
*
* @param mask : 컨볼루션에 사용할 마스크
*/
Mat makeFilter(vvf& mask) {
	int len = mask.size();
	Mat filter = Mat::ones(len, len, CV_64F);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			*filter.ptr<double>(i, j) = mask[i][j];
		}
	}

	return filter;
}

int main() {
	vvf maskPositive45 = {
		{6, -1, -1, -1, -1, -1,-1},
		{-1, 6, -1, -1, -1, -1,-1},
		{-1, -1, 6, -1, -1, -1,-1},
		{-1, -1, -1, 6, -1, -1,-1},
		{-1, -1, -1, -1, 6, -1,-1},
		{-1, -1, -1, -1, -1, 6,-1},
		{-1, -1, -1, -1, -1, -1,6}
	};

	vvf maskNegative45 = {
		{-1, -1, -1, -1, -1, -1,6},
		{-1, -1, -1, -1, -1, 6,-1},
		{-1, -1, -1, -1, 6, -1,-1},
		{-1, -1, -1, 6, -1, -1,-1},
		{-1, -1, 6, -1, -1, -1,-1},
		{-1, 6, -1, -1, -1, -1,-1},
		{6, -1, -1, -1, -1, -1,-1}
	};

	// 원본 영상
	Mat src = imread("HW5-2.jpg", IMREAD_GRAYSCALE);
	if (src.empty())
		return -1;

	Mat dst1, bin_dst1, dst2, bin_dst2;
	int dtype = CV_8U;
	int thresh = 250;

	// +45도 컨볼루션
	Mat positive45Kernel = makeFilter(maskPositive45);
	cout << positive45Kernel << "\n";
	filter2D(src, dst1, -1, positive45Kernel, Point(-1, -1), 0, BORDER_DEFAULT); // +45
	normalize(dst1, bin_dst1, 0, 255, NORM_MINMAX, dtype);
	threshold(dst1, bin_dst1, thresh, 255, THRESH_BINARY); //thresholding

	// -45도 컨볼루션
	Mat negative45Kernel = makeFilter(maskNegative45);
	cout << negative45Kernel << "\n";
	filter2D(src, dst2, -1, negative45Kernel, Point(-1, -1), 0, BORDER_DEFAULT); // -45
	normalize(dst2, bin_dst2, 0, 255, NORM_MINMAX, dtype);
	threshold(dst2, bin_dst2, thresh, 255, THRESH_BINARY); //thresholding

	// print result
	Size size(800, 600);
	showMyWindow("+45 대각선 검출", size, bin_dst1);
	showMyWindow("-45 대각선 검출", size, bin_dst2);

	waitKey(0);
}

