#ifndef GABORFILTER_H
#define GABORFILTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

class GaborFilter {

public:
	int kernel_size = 21;
	int pos_sigma = 5;
	int pos_lm = 50;
	int pos_th = 0;
	int pos_psi = 90;
	cv::Mat src_f;
	cv::Mat dest;
	cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps);
	void Process(int, void*);

};

#endif
