//============================================================================
// Name        : NoiseRemoval.cpp
// Author      : Seungbin Ahn
// Version     : 1.0
// Description : Noise Removal
//============================================================================

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <cstdlib>

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

/**
* Make Salt And Pepper Nois image
*
* @param src : src image
* @param dest : dest image
* @param sigma: noise ratio
*/
void makeSaltAndPepperNoise(Mat& src, Mat& dest, double ratio) {
	srand((unsigned int)time(0));
	src.copyTo(dest); // copy image

	int rows = src.rows; // 행 크기
	int cols = src.cols; // 열 크기
	int ch = src.channels(); // 채널

	// 전체 이미지 화소 대비 noise 비율 계산
	int num = (int)((double)(rows * cols * ch) * ratio);

	for (int i = 0; i < num; i++) {
		int r = (rand() % rows); // 대상 행 임의 선정
		int c = (rand() % cols); // 대상 열 임의 선정
		int _ch = (rand() % ch); // 대상 채널 임의 선정

		uchar* pixel = dest.ptr<uchar>(r) + (c * ch) + _ch; // noise로 바꿀 화소의 위치

		// salt and pepper noise 추가
		if (rand() % 2 == 1) *pixel = 255;
		else pixel = 0;
	}
}

/**
* Make Gaussian Nois image
*
* @param src : src image
* @param dest : dest image
* @param sigma: sdv of gaussian noise
*/
void makeGaussianNoise(Mat& src, Mat& dest, double sigma) {
	Mat noise(src.size(), CV_32SC3);
	// 평균이 0, 표준편차가 sigma인 가우시안 분포 생성
	randn(noise, 10, sigma);
	// 가우시안 노이즈 추가
	add(src, noise, dest, Mat(), CV_8UC3);
}

void main() {
	// 원본 영상
	Mat src = imread("Lena.jpg", 1);

	// Salt and Pepper 영상 생성
	Mat spImg = imread("Lena.jpg", 1);
	makeSaltAndPepperNoise(src, spImg, 0.05);

	// Gaussian noise 영상 생성
	Mat gImg;
	makeGaussianNoise(src, gImg, 1.5);

	// src, noise 영상 출력
	Size size(300, 300);
	showMyWindow("src", size, src);
	showMyWindow("salt_and_pepper_noise_img", size, spImg);
	showMyWindow("gaussian_noise_img", size, gImg);

	// Salt and pepper noise에 필터 적용
	{
		// Salt and pepper noise 영상에 median filter 적용
		Mat sp_median_img;
		medianBlur(spImg, sp_median_img, 3);
		showMyWindow("sp_mdian_filter", size, sp_median_img);

		// Salt and pepper noise 영상에 gaussian filter 적용
		Mat sp_gaussian_img;
		GaussianBlur(spImg, sp_gaussian_img, Size(11, 11), 0, 0);
		showMyWindow("sp_gaussian_filter", size, sp_gaussian_img);
	}

	// Gaussian noise에 필터 적용
	{
		// Salt and pepper noise 영상에 median filter 적용
		Mat g_median_img;
		medianBlur(gImg, g_median_img, 3);
		showMyWindow("g_mdian_filter", size, g_median_img);

		// gaussian noise noise 영상에 gaussian filter 적용
		Mat g_gaussian_img;
		GaussianBlur(gImg, g_gaussian_img, Size(11, 11), 0, 0);
		showMyWindow("g_gaussian_filter", size, g_gaussian_img);
	}

	waitKey(0);
}

