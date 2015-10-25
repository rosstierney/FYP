#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Histogram.h"
#include "GaborFilter.h"
#include "Thinning.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Image.h"

using namespace cv;
using namespace std;

void thinner(Mat &img)
{
	// thinning
	Thinning thin;
	thin.thinning(img);

	namedWindow("Thinning", CV_WINDOW_AUTOSIZE); //create a window with the name "Thinning"
	cv::imshow("Thinning", img);
}


cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps)
{
	int hks = (ks - 1) / 2;
	double theta = th*CV_PI / 180;
	double psi = ps*CV_PI / 180;
	double del = 2.0 / (ks - 1);
	double lmbd = lm;
	double sigma = sig / ks;
	double x_theta;
	double y_theta;
	cv::Mat kernel(ks, ks, CV_32F);
	for (int y = -hks; y <= hks; y++)
	{
		for (int x = -hks; x <= hks; x++)
		{
			x_theta = x*del*cos(theta) + y*del*sin(theta);
			y_theta = -x*del*sin(theta) + y*del*cos(theta);
			kernel.at<float>(hks + y, hks + x) = (float)exp(-0.5*(pow(x_theta, 2) + pow(y_theta, 2)) / pow(sigma, 2))* cos(2 * CV_PI*x_theta / lmbd + psi);
		}
	}
	return kernel;
}

int kernel_size = 21;
int pos_sigma = 2;
int pos_lm = 35;
int pos_th = 0;
int pos_psi = 90;
cv::Mat src_f;
cv::Mat dest;
cv::Mat dst;
string fileName = "C:/Users/Ross/Downloads/Fingerprint FYP/Files_FingerPrint/FingerPrint1.bmp";


void Process(int, void *)
{

	double sig = pos_sigma;
	double lm = 0.5 + pos_lm / 100.0;
	double th = pos_th;
	double ps = pos_psi;
	cv::Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
	cv::filter2D(src_f, dest, CV_32F, kernel);
	cv::imshow("Process window", dest);
	cv::Mat Lkernel(kernel_size * 20, kernel_size * 20, CV_32F);
	cv::resize(kernel, Lkernel, Lkernel.size());
	Lkernel /= 2.;
	Lkernel += 0.5;
	cv::imshow("Kernel", Lkernel);
	cv::Mat mag;
	cv::pow(dest, 2.0, mag);
	cv::imshow("Mag", mag);

	IplImage ipl = dest;

	cv::Mat m = cv::cvarrToMat(&ipl);  // default additional arguments: don't copy data.

	// binarization
	cvSmooth(&ipl, &ipl, CV_GAUSSIAN, 9, 9, 0);
	cv::imshow("Smoothing", m);


	//threshold(dest, dst, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	threshold(m, m, 1, 255, CV_THRESH_BINARY);
	//cvAdaptiveThreshold(dest, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 1);
	namedWindow("Binarized", CV_WINDOW_AUTOSIZE); //create a window with the name "Binarized"
	imshow("Binarized", m); //display the image which is stored in the 'img' in the "Binarized" window

	//thinner(dst);
	
}


int main(int argc, const char** argv)
{
	cv::Mat image = cv::imread(fileName);
	cv::imshow("Src", image);
	cv::Mat gab;
	cv::cvtColor(image, gab, CV_BGR2GRAY);
	gab.convertTo(src_f, CV_32F, 1.0 / 255, 0);
	if (!kernel_size % 2)
	{
		kernel_size += 1;
	}

	Histogram histogram;
	cv::imshow("Histogram Equalization", histogram.showHistogram(image));

	cv::namedWindow("Process window", 1);
	cv::createTrackbar("Sigma", "Process window", &pos_sigma, kernel_size, Process);
	cv::createTrackbar("Lambda", "Process window", &pos_lm, 100, Process);
	cv::createTrackbar("Theta", "Process window", &pos_th, 180, Process);
	cv::createTrackbar("Psi", "Process window", &pos_psi, 360, Process);
	Process(0, 0);

	waitKey(0); //wait infinite time for a keypress
	//destroyWindow("Binarized"); //destroy the window with the name, "Binarized"
	//destroyWindow("Thinning"); //destroy the window with the name, "Thinning"

	return 0;
}