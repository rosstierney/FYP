#ifndef WATERSHED_H_INCLUDED
#define WATERSHED_H_INCLUDED

#include "opencv2/opencv.hpp"
#include <string>

using namespace cv;
using namespace std;

class WatershedSegmenter {
private:
	cv::Mat markers;
public:
	void setMarkers(const cv::Mat& markerImage) {
		// Convert to image of ints
		markerImage.convertTo(markers, CV_32S);
	}
	cv::Mat process(const cv::Mat &image) {
		// Apply watershed
		cv::watershed(image, markers);
		return markers;
	}
	// Return result in the form of an image
	cv::Mat getSegmentation() {
		cv::Mat tmp;
		// all segment with label higher than 255
		// will be assigned value 255
		markers.convertTo(tmp, CV_8U);
		return tmp;
	}
	// Return watershed in the form of an image
	cv::Mat getWatersheds() {
		cv::Mat tmp;
		// Each pixel p is transformed into
		// 255p+255 before conversion
		markers.convertTo(tmp, CV_8U, 255, 255);
		return tmp;
	}
};

#endif
