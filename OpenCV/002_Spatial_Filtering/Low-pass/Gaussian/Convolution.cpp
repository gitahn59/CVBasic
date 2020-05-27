//============================================================================
// Name        : Convolution.cpp
// Author      : Seungbin Ahn
// Version     : 1.0
// Description : Convolution
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

/**
* 크기 N의 수직 모션 필터를 생성한다.
*
* @param N : motion 필터의 크기
*/
Mat makeVerMotionFilter(int N) {
	vvf mask(N, vector<double>(N, 0));
	// 중앙 수직 열에만 값 할당
	for (int n = 0; n < N; n++) {
		mask[n][N / 2] = 1.0 / N;
	}

	int len = mask.size();
	Mat filter = Mat::ones(len, len, CV_64F);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			*filter.ptr<double>(i, j) = mask[i][j];
		}
	}

	return filter;
}

void main() {
	// 필터 값 vector
	vvf box = {
		{1.0 / 9, 1.0 / 9, 1.0 / 9},
		{1.0 / 9, 1.0 / 9, 1.0 / 9},
		{1.0 / 9, 1.0 / 9, 1.0 / 9}
	};

	vvf gaussian = {
		{0,		 0,		 0.0002, 0,		 0	   },
		{0,		 0.0113, 0.0837, 0.0113, 0	   },
		{0.0002, 0.0837, 0.6187, 0.0837, 0.0002},
		{0,		 0.0113, 0.0837, 0.0113, 0	   },
		{0,		 0,		 0.0002, 0,		 0	   },
	};

	vvf sharpening = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};

	vvf horEdge = {
		{ 1, 1, 1 },
		{ 0, 0, 0 },
		{ -1, -1, -1 }
	};

	vvf verEdge = {
		{1, 0, -1},
		{1, 0, -1},
		{1, 0, -1}
	};

	vvf motion = {
		{0.0304, 0.0501, 0, 0, 0},
		{0.0501, 0.1771, 0.0519, 0, 0},
		{0,	0.0519, 0.1771, 0.0519, 0},
		{0, 0, 0.0519, 0.1771, 0.0501},
		{0, 0, 0, 0.0501, 0.0304}
	};

	// 원본 영상
	Mat src = imread("bird.png");
	Mat boxImg, gaussianImg, sharpeningImg, horEdgeImg, verEdgeImg, motionImg, verMotionImg;

	// 컨볼루션 적용
	filter2D(src, boxImg, -1, makeFilter(box), Point(-1, -1), 0, BORDER_DEFAULT); // 박스
	filter2D(src, gaussianImg, -1, makeFilter(gaussian), Point(-1, -1), 0, BORDER_DEFAULT); // 가우시안
	filter2D(src, sharpeningImg, -1, makeFilter(sharpening), Point(-1, -1), 0, BORDER_DEFAULT); // 샤프닝
	filter2D(src, horEdgeImg, -1, makeFilter(horEdge), Point(-1, -1), 0, BORDER_DEFAULT); // 수평 에지
	filter2D(src, verEdgeImg, -1, makeFilter(verEdge), Point(-1, -1), 0, BORDER_DEFAULT); // 수직 에지
	filter2D(src, motionImg, -1, makeFilter(motion), Point(-1, -1), 0, BORDER_DEFAULT); // 모션
	filter2D(src, verMotionImg, -1, makeVerMotionFilter(31), Point(-1, -1), 0, BORDER_DEFAULT); // 큰 수직 모션

	// print result
	Size size(400, 280);
	showMyWindow("src", size, src);
	showMyWindow("box", size, boxImg);
	showMyWindow("gaussian", size, gaussianImg);
	showMyWindow("sharpening", size, sharpeningImg);
	showMyWindow("수평 에지", size, horEdgeImg);
	showMyWindow("수직 에지", size, verEdgeImg);
	showMyWindow("motion", size, motionImg);
	showMyWindow("수직 motion", size, verMotionImg);

	waitKey(0);
}

