//============================================================================
// Name        : edgedectector.cpp
// Author      : Seungbin Ahn
// Version     : 1.0
// Description : Edge Dectector
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
* Sobel 연산자를 컨볼루션한 결과를 그린다
*
* @param srcImage : 연산에 사용될 원본 이미지
*/
void drawSobel(Mat& srcImage);

/**
* Laplician 연산자를 컨볼루션한 결과를 그린다
*
* @param srcImage : 연산에 사용될 원본 이미지
*/
void drawLaplician(Mat& srcImage);

/**
* Marr-Hildreth 에지 검출 알고리즘을 사용한 결과를 그린다
*
* @param srcImage : 연산에 사용될 원본 이미지
*/
void drawLoG(Mat& srcImage);

/**
* 2차 미분의 결과를 이용해 0교차점을 탐색하여 dst 를 생성한다
*
* @param srcImage : 연산에 사용될 원본 이미지
*/
void ZeroCrossing(Mat& src, Mat& dst, int th);

int main() {
	// 원본 영상
	Mat src = imread("HW5-1.jpg", IMREAD_GRAYSCALE);
	//Mat src = imread("lena.jpg", IMREAD_GRAYSCALE);
	if (src.empty())
		return -1;

	drawSobel(src);
	drawLaplician(src);
	drawLoG(src);

	waitKey(0);
}

void drawSobel(Mat& srcImage) {
	int ksize = 3;
	int ddepth = CV_32F;	// 깊이에 대한 자료형 변경
	Mat dstGx, dstGy;
	Sobel(srcImage, dstGx, ddepth, 1, 0, ksize);    // dx
	Sobel(srcImage, dstGy, ddepth, 0, 1, ksize);	// dy

	int dtype = CV_8U;
	int thresh = 70;

	Mat dstImageGx;
	normalize(abs(dstGx), dstImageGx, 0, 255, NORM_MINMAX, dtype);    // |dstGx|
	threshold(abs(dstGx), dstImageGx, thresh, 255, THRESH_BINARY);
	imshow("Sobel - |dstGx|", dstImageGx);

	Mat dstImageGy;
	normalize(abs(dstGy), dstImageGy, 0, 255, NORM_MINMAX, dtype);	// |dstGy|	
	threshold(abs(dstGy), dstImageGy, thresh, 255, THRESH_BINARY);
	imshow("Sobel - |dstGy|", dstImageGy);

	Mat dstMag;
	magnitude(dstGx, dstGy, dstMag);  // 단순화하면.. |dstGx| + |dstGy|
	Mat dstImageGxy;
	normalize(dstMag, dstImageGxy, 0, 255, NORM_MINMAX, dtype);
	threshold(dstMag, dstImageGxy, thresh, 255, THRESH_BINARY);
	imshow("Sobel - |dstGx| + |dstGy|", dstImageGxy);
}

void drawLaplician(Mat& srcImage) {
	int ksize = 15;
	Mat blurImage;
	GaussianBlur(srcImage, blurImage, Size(ksize, ksize), 0.0);  // srcImage를 15 x 15의 가우시안 필터로 스무딩

	Mat lapImage;
	Laplacian(blurImage, lapImage, CV_32F, ksize);  // 가우시안 스무딩 된 영상에 대해 라플라시안 필터링 수행

	Mat dstImage;
	ZeroCrossing(lapImage, dstImage, 10);  // 라플라시안 결과에 대해 영교차점 체크

	imshow("Laplacian", dstImage);
}

void drawLoG(Mat& srcImage) {
	const int ksize = 9;
	float logArr[ksize * ksize];
	int   s, t, k = 0;
	float g;
	float sigma = 1.4f;
	//	float sigma= 0.3f*(ksize/2-1.0f) + 0.8f;

	for (s = -ksize / 2; s <= ksize / 2; s++)   // ksize x ksize 크기의 LoG 필터 커널 생성
	{
		for (t = -ksize / 2; t <= ksize / 2; t++)
		{
			g = exp(-((float)s * s + (float)t * t) / (2 * sigma * sigma));
			g *= (1 - ((float)s * s + (float)t * t) / (2 * sigma * sigma));
			g /= (3.141592f * sigma * sigma * sigma * sigma);
			logArr[k++] = -g;
		}
	}

	Mat logKernel(ksize, ksize, CV_32F, logArr);
	cout << "logKernel=" << logKernel << endl;

	Mat logImage;
	filter2D(srcImage, logImage, CV_32F, logKernel);  // LoG 필터링

	Mat dstImage;
	ZeroCrossing(logImage, dstImage, 0);  // 0 교차점 검출
	imshow("Marr_Hildreth", dstImage);
}


void ZeroCrossing(Mat& src, Mat& dst, int th)
{
	int x, y;
	double a, b;

	Mat zeroCrossH(src.size(), CV_32F, Scalar::all(0));  // 가로 방향의 0-교차 검출
	Mat_<float> _src(src);	// src를 복사하여 _src 생성

	for (x = 1; x < src.rows - 1; x++)
	{
		for (y = 1; y < src.cols - 1; y++)
		{
			// (+.-), (-,+), (+,0,-), (-,0,+) 인 경우를 검출하여 교차하는 정도를 zeroCrossH에 저장
			a = _src(x, y);     		// 가로 방향의 인접한 두 픽셀 선택
			b = _src(x, y + 1);

			if (a == 0)   			// 해당 픽셀의 값이 0 이면 그 왼쪽의 픽셀을 다시 선택.
							// (+,0,-) 또는 (-,0,+)인 경우를 검출하기 위함
				a = _src(x, y - 1);

			if (a * b < 0)			// 가로 방향의 인접한 두 픽셀의 부호가 다르다면
							// 두 픽셀의 절대값의 합을 저장
				zeroCrossH.at<float>(x, y) = fabs(a) + fabs(b);

			else				// 가로 방향의 인접한 두 픽셀의 부호가 같으면 0 저장
				zeroCrossH.at<float>(x, y) = 0;

		}
	}

	Mat zeroCrossV(src.size(), CV_32F, Scalar::all(0));  // 세로 방향의 0-교차 검출
	for (x = 1; x < src.rows - 1; x++)
	{
		for (y = 1; y < src.cols - 1; y++)
		{
			a = _src(x, y);
			b = _src(x + 1, y);

			if (a == 0)
				a = _src(x - 1, y);
			if (a * b < 0)
				zeroCrossV.at<float>(x, y) = fabs(a) + fabs(b);
			else
				zeroCrossV.at<float>(x, y) = 0;
		}
	}

	Mat zeroCross(src.size(), CV_32F, Scalar::all(0));
	add(zeroCrossH, zeroCrossV, zeroCross);  // zeroCross = zeroCrossH + zeroCrossV
	threshold(zeroCross, dst, th, 255, THRESH_BINARY);
}