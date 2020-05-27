//============================================================================
// Name        : HistogramBackProject.cpp
// Author      : Seungbin Ahn
// Version     : 1.0
// Description : back_project
//============================================================================

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

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

	Mat srcImage = imread("fruits.jpg");
	Mat hsvImage;
	cvtColor(srcImage, hsvImage, COLOR_BGR2HSV);

	vector<Mat> planes;
	split(hsvImage, planes); // h, s, v �� �迭�� �и�
	Mat hueImage = planes[0]; // hue �̹����� �ٲ�

	Rect roi(400, 150, 50, 50);
	rectangle(srcImage, roi, Scalar(0, 0, 255), 2);
	Mat roiImage = hueImage(roi); // ���ɿ��� �и��ؼ� roiImage ����

	int histSize = 256;
	float hValue[] = { 0,256 };
	const float* ranges[] = { hValue };
	int channels = 0;
	int dims = 1;

	Mat hist, hueImage2, backProject;
	calcHist(&roiImage, 1, &channels, Mat(), hist, dims, &histSize, ranges); //roiImage�� ���� ������׷� ���

	hueImage.convertTo(hueImage2, CV_32F); // �ڷ����� CV_32F�� ��ȯ�Ͽ� hueImage2 ����
	calcBackProject(&hueImage2, 1, &channels, hist, backProject, ranges); // hueImage���� BackPrject ���

	showMyWindow("src", Size(400, 400), srcImage);
	showMyWindow("back", Size(400, 400), backProject);

	waitKey(0);
	return 0;
}



