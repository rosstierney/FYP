#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Histogram.h"
#include "GaborFilter.h"
#include "Thinning.h"
#include "Watershed.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Image.h"
#include <sstream>

using namespace std;

//global variables

int kernel_size = 21;
cv::Mat src_f;
cv::Mat otsu;
string fileName = "C:/Users/Ross/Downloads/Fingerprint FYP/Files_FingerPrint/FingerPrint1.bmp";
string fileName2 = "C:/Users/Ross/Documents/Visual Studio 2015/Projects/Test/src/1.bmp";
string fileName3 = "C:/Users/Ross/OneDrive/Documents/PNGfingerprint/PNG/1_2.png";

//thinning function
void thinner(Mat &img)
{
	// thinning
	Thinning thin;
	thin.thinning(img);

	namedWindow("Thinning", CV_WINDOW_AUTOSIZE); //create a window with the name "Thinning"
	cv::imshow("Thinning", img);
}

void calculateOrientations(Mat gradientX, Mat gradientY) {
	// Create container element
	Mat orientation = Mat(gradientX.rows, gradientX.cols, CV_32F);

	// Calculate orientations of gradients --> in degrees
	// Loop over all matrix values and calculate the accompagnied orientation
	for (int i = 0; i < gradientX.rows; i++) {
		for (int j = 0; j < gradientX.cols; j++) {
			// Retrieve a single value
			float valueX = gradientX.at<float>(i, j);
			float valueY = gradientY.at<float>(i, j);
			// Calculate the corresponding single direction, done by applying the arctangens function
			float result = fastAtan2(valueX, valueY);
			// Store in orientation matrix element
			orientation.at<float>(i, j) = result;
		}
	}

	imshow("Orientation",orientation);
}

void backgroundSegmenter()
{
	// Read Input Image
	Mat src = imread(fileName3);

	// Convert the input image to gray-scale
	Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);

	// Threshold the gray-input image
	Mat src_thresh;
	threshold(src_gray, src_thresh, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// Detect the foreground.
	Mat fg;
	erode(src_thresh, fg, Mat(), Point(-1, -1), 6);
	imshow("Foreground", fg);

	// Detect the background
	Mat bg;
	dilate(src_thresh, bg, Mat(), Point(-1, -1), 6);
	threshold(bg, bg, 1, 128, THRESH_BINARY_INV);
	imshow("Background", bg);

	// Get the markers
	Mat markers(src_thresh.size(), CV_8U, Scalar(0));
	markers = fg - bg;
	imshow("Markers", markers);

	// Create watershed segmentation object
	WatershedSegmenter segmenter;

	// Set markers and process
	segmenter.setMarkers(markers);
	segmenter.process(src);

	// Get segmentation result
	Mat seg = segmenter.getSegmentation();

	// Display the result
	imshow("Segmenter", seg);

	// Get watersheds
	Mat ws = segmenter.getWatersheds();

	// Display the result
	imshow("Watershed", ws);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Find contours
	Mat ws_negate = ~ws;
	findContours(ws_negate, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}

	/// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros(ws.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		drawContours(drawing, contours_poly, i, Scalar(0, 0, 255), 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
		circle(drawing, center[i], (int)radius[i], Scalar(0, 255, 0), 2, 8, 0);
	}

	/// Show in a window
	imshow("Contours", drawing);
}
 
Mat enhanceImg(Mat& inMat) {

	int ks = 47;
	int hks = (ks - 1) / 2;
	int kernel_size = 21;
	double sig = 3;
	double th = 10;
	double ps = 90;
	double lm = 0.5 + ps / 100.0;
	double theta = th*CV_PI / 180;
	double psi = ps*CV_PI / 180;
	double del = 2.0 / (ks - 1);
	double sigma = sig / ks;
	double x_theta;
	double y_theta;

	Mat src_f, dest;

	//Mat image = imread(fileName2, 1), dest, src, src_f;

	//imshow("Src", image);

	cvtColor(inMat, inMat, CV_BGR2GRAY);
	inMat.convertTo(src_f, CV_32F, 1.0 / 255, 0);
	if (!ks % 2)
	{
		ks += 1;
	}

	Mat kernel(ks, ks, CV_32F);

	for (int y = -hks; y <= hks; y++)
	{
		for (int x = -hks; x <= hks; x++)
		{
			x_theta = x*del*cos(theta) + y*del*sin(theta);
			y_theta = -x*del*sin(theta) + y*del*cos(theta);
			kernel.at<float>(hks + y, hks + x) = (float)exp(-0.5*(pow(x_theta, 2) + pow(y_theta, 2)) / pow(sigma, 2))* cos(2 * CV_PI*x_theta / lm + psi);
		}
	}

	filter2D(src_f, dest, CV_32F, kernel);
	imshow("Gabor", dest);
	Mat Lkernel(kernel_size * 20, kernel_size * 20, CV_32F);
	resize(kernel, Lkernel, Lkernel.size());
	Lkernel /= 2.;
	Lkernel += 0.5;
	imshow("Kernel", Lkernel);
	Mat mag;
	pow(dest, 2.0, mag);
	imshow("Mag", mag);

	return dest;
}

int main(int argc, const char** argv)
{
	//read in file as matrix
	cv::Mat image = cv::imread(fileName3);
	
	//convert color to grey
	//cvtColor(image, image, CV_BGR2GRAY);
	imshow("Original Image", image);

	//Gabor Filter
	enhanceImg(image);


	//use Histogram object for equalization
	/*Histogram histogram;
	cv::imshow("Histogram Equalization", histogram.showHistogram(image));*/

	//convert image to Ipl
	IplImage ipl = image;
	cv::Mat binary = cv::cvarrToMat(&ipl);  // default additional arguments: don't copy data.

	// Gaussian Filter
	cvSmooth(&ipl, &ipl, CV_GAUSSIAN, 9, 9, 0);
	cv::imshow("Smoothing", binary);

	//Normal binary threshold
	//threshold(m, m, 30, 255, CV_THRESH_BINARY);

	//Otsu Binary thresholding
	threshold(image, otsu, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	//create window and show
	namedWindow("Binarized Otsu", CV_WINDOW_AUTOSIZE); //create a window with the name "Binarized"
	imshow("Binarized Otsu", otsu);

	//convert m to CvMat
	CvMat cvmat = binary;

	//apply guassian adaptive threshold
	cvAdaptiveThreshold(&cvmat, &cvmat, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 13, 1);

	//Normal binarized image
	namedWindow("Binarized", CV_WINDOW_AUTOSIZE); //create a window with the name "Binarized"
	imshow("Binarized", binary); //display the image which is stored in the 'img' in the "Binarized" window


	calculateOrientations(binary, binary);
	
	//call thinning method
	thinner(binary);

	/*cv::Mat image1 = cv::imread(fileName2);
	imshow("originalimage", image1);*/

	//segement the background
	backgroundSegmenter();


	
	/*// Create markers image
	cv::Mat markers(m.size(), CV_8U, cv::Scalar(-1));
	//Rect(topleftcornerX, topleftcornerY, width, height);
	//top rectangle
	markers(Rect(0, 0, m.cols, 10)) = Scalar::all(1);
	//bottom rectangle
	markers(Rect(0, m.rows - 10, m.cols, 10)) = Scalar::all(1);
	//left rectangle
	markers(Rect(0, 0, 10, m.rows)) = Scalar::all(1);
	//right rectangle
	markers(Rect(m.cols - 10, 0, 10, m.rows)) = Scalar::all(1);
	//centre rectangle
	int centreW = m.cols / 3;
	int centreH = m.rows / 3;
	markers(Rect((m.cols / 2) - (centreW / 2), (m.rows / 2) - (centreH / 2), centreW, centreH)) = Scalar::all(3);
	markers.convertTo(markers, CV_BGR2GRAY);
	imshow("markers", markers);

	//Create watershed segmentation object
	Mat dest1;
	WatershedSegmenter segmenter;
	segmenter.setMarkers(markers);
	cv::Mat wshedMask = segmenter.process(image);
	cv::Mat mask;
	convertScaleAbs(wshedMask, mask, 1, 0);
	double thresh = threshold(mask, mask, 1, 255, THRESH_BINARY);
	bitwise_and(m, m, dest1, mask);
	dest1.convertTo(dest1, CV_8U);

	imshow("final_result", dest1);*/

	/*cv::namedWindow("Process window", 1);
	cv::createTrackbar("Sigma", "Process window", &pos_sigma, kernel_size, Process);
	cv::createTrackbar("Lambda", "Process window", &pos_lm, 100, Process);
	cv::createTrackbar("Theta", "Process window", &pos_th, 180, Process);
	cv::createTrackbar("Psi", "Process window", &pos_psi, 360, Process);
	Process(0, 0);*/

	waitKey(0); //wait infinite time for a keypress
	//destroyWindow("Binarized"); //destroy the window with the name, "Binarized"
	//destroyWindow("Thinning"); //destroy the window with the name, "Thinning"

	return 0;
}



