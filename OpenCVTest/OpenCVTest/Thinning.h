#ifndef THINNING_H
#define THINNING_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Thinning {

public:
	Thinning();
	void thinningIteration(cv::Mat& im, int iter);
	void thinning(cv::Mat& im);


private:
	string fileName = "C:/Users/Ross/Downloads/Fingerprint FYP/Files_FingerPrint/fingerprint1.bmp";

};
#endif