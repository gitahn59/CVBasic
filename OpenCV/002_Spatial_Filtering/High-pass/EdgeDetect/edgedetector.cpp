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

// filter ���� �����ϴ� 2���� vector
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
* Sobel �����ڸ� ��������� ����� �׸���
*
* @param srcImage : ���꿡 ���� ���� �̹���
*/
void drawSobel(Mat& srcImage);

/**
* Laplician �����ڸ� ��������� ����� �׸���
*
* @param srcImage : ���꿡 ���� ���� �̹���
*/
void drawLaplician(Mat& srcImage);

/**
* Marr-Hildreth ���� ���� �˰����� ����� ����� �׸���
*
* @param srcImage : ���꿡 ���� ���� �̹���
*/
void drawLoG(Mat& srcImage);

/**
* 2�� �̺��� ����� �̿��� 0�������� Ž���Ͽ� dst �� �����Ѵ�
*
* @param srcImage : ���꿡 ���� ���� �̹���
*/
void ZeroCrossing(Mat& src, Mat& dst, int th);

int main() {
	// ���� ����
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
	int ddepth = CV_32F;	// ���̿� ���� �ڷ��� ����
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
	magnitude(dstGx, dstGy, dstMag);  // �ܼ�ȭ�ϸ�.. |dstGx| + |dstGy|
	Mat dstImageGxy;
	normalize(dstMag, dstImageGxy, 0, 255, NORM_MINMAX, dtype);
	threshold(dstMag, dstImageGxy, thresh, 255, THRESH_BINARY);
	imshow("Sobel - |dstGx| + |dstGy|", dstImageGxy);
}

void drawLaplician(Mat& srcImage) {
	int ksize = 15;
	Mat blurImage;
	GaussianBlur(srcImage, blurImage, Size(ksize, ksize), 0.0);  // srcImage�� 15 x 15�� ����þ� ���ͷ� ������

	Mat lapImage;
	Laplacian(blurImage, lapImage, CV_32F, ksize);  // ����þ� ������ �� ���� ���� ���ö�þ� ���͸� ����

	Mat dstImage;
	ZeroCrossing(lapImage, dstImage, 10);  // ���ö�þ� ����� ���� �������� üũ

	imshow("Laplacian", dstImage);
}

void drawLoG(Mat& srcImage) {
	const int ksize = 9;
	float logArr[ksize * ksize];
	int   s, t, k = 0;
	float g;
	float sigma = 1.4f;
	//	float sigma= 0.3f*(ksize/2-1.0f) + 0.8f;

	for (s = -ksize / 2; s <= ksize / 2; s++)   // ksize x ksize ũ���� LoG ���� Ŀ�� ����
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
	filter2D(srcImage, logImage, CV_32F, logKernel);  // LoG ���͸�

	Mat dstImage;
	ZeroCrossing(logImage, dstImage, 0);  // 0 ������ ����
	imshow("Marr_Hildreth", dstImage);
}


void ZeroCrossing(Mat& src, Mat& dst, int th)
{
	int x, y;
	double a, b;

	Mat zeroCrossH(src.size(), CV_32F, Scalar::all(0));  // ���� ������ 0-���� ����
	Mat_<float> _src(src);	// src�� �����Ͽ� _src ����

	for (x = 1; x < src.rows - 1; x++)
	{
		for (y = 1; y < src.cols - 1; y++)
		{
			// (+.-), (-,+), (+,0,-), (-,0,+) �� ��츦 �����Ͽ� �����ϴ� ������ zeroCrossH�� ����
			a = _src(x, y);     		// ���� ������ ������ �� �ȼ� ����
			b = _src(x, y + 1);

			if (a == 0)   			// �ش� �ȼ��� ���� 0 �̸� �� ������ �ȼ��� �ٽ� ����.
							// (+,0,-) �Ǵ� (-,0,+)�� ��츦 �����ϱ� ����
				a = _src(x, y - 1);

			if (a * b < 0)			// ���� ������ ������ �� �ȼ��� ��ȣ�� �ٸ��ٸ�
							// �� �ȼ��� ���밪�� ���� ����
				zeroCrossH.at<float>(x, y) = fabs(a) + fabs(b);

			else				// ���� ������ ������ �� �ȼ��� ��ȣ�� ������ 0 ����
				zeroCrossH.at<float>(x, y) = 0;

		}
	}

	Mat zeroCrossV(src.size(), CV_32F, Scalar::all(0));  // ���� ������ 0-���� ����
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