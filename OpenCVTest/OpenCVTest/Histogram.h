#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Histogram {

public:
	Histogram();
	Mat showHistogram(Mat&);

private:
	string fileName = "C:/Users/Ross/Downloads/Fingerprint FYP/Files_FingerPrint/fingerprint1.bmp";
	
};
#endif