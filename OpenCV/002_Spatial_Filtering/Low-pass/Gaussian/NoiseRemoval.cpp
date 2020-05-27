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

	int rows = src.rows; // �� ũ��
	int cols = src.cols; // �� ũ��
	int ch = src.channels(); // ä��

	// ��ü �̹��� ȭ�� ��� noise ���� ���
	int num = (int)((double)(rows * cols * ch) * ratio);

	for (int i = 0; i < num; i++) {
		int r = (rand() % rows); // ��� �� ���� ����
		int c = (rand() % cols); // ��� �� ���� ����
		int _ch = (rand() % ch); // ��� ä�� ���� ����

		uchar* pixel = dest.ptr<uchar>(r) + (c * ch) + _ch; // noise�� �ٲ� ȭ���� ��ġ

		// salt and pepper noise �߰�
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
	// ����� 0, ǥ�������� sigma�� ����þ� ���� ����
	randn(noise, 10, sigma);
	// ����þ� ������ �߰�
	add(src, noise, dest, Mat(), CV_8UC3);
}

void main() {
	// ���� ����
	Mat src = imread("Lena.jpg", 1);

	// Salt and Pepper ���� ����
	Mat spImg = imread("Lena.jpg", 1);
	makeSaltAndPepperNoise(src, spImg, 0.05);

	// Gaussian noise ���� ����
	Mat gImg;
	makeGaussianNoise(src, gImg, 1.5);

	// src, noise ���� ���
	Size size(300, 300);
	showMyWindow("src", size, src);
	showMyWindow("salt_and_pepper_noise_img", size, spImg);
	showMyWindow("gaussian_noise_img", size, gImg);

	// Salt and pepper noise�� ���� ����
	{
		// Salt and pepper noise ���� median filter ����
		Mat sp_median_img;
		medianBlur(spImg, sp_median_img, 3);
		showMyWindow("sp_mdian_filter", size, sp_median_img);

		// Salt and pepper noise ���� gaussian filter ����
		Mat sp_gaussian_img;
		GaussianBlur(spImg, sp_gaussian_img, Size(11, 11), 0, 0);
		showMyWindow("sp_gaussian_filter", size, sp_gaussian_img);
	}

	// Gaussian noise�� ���� ����
	{
		// Salt and pepper noise ���� median filter ����
		Mat g_median_img;
		medianBlur(gImg, g_median_img, 3);
		showMyWindow("g_mdian_filter", size, g_median_img);

		// gaussian noise noise ���� gaussian filter ����
		Mat g_gaussian_img;
		GaussianBlur(gImg, g_gaussian_img, Size(11, 11), 0, 0);
		showMyWindow("g_gaussian_filter", size, g_gaussian_img);
	}

	waitKey(0);
}

