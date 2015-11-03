#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>

#include "..\lib\binarisation\AdaptiveLocalThreshold.h"
#include "..\lib\optimisation\Ideka.h"
#include "..\lib\thinning\GuoHall.h"
#include "..\lib\extraction\CrossingNumber.h"
#include "..\lib\extraction\Filter.h"
#include "..\Test\Watershed.h"

using namespace cv;
using namespace std;
string fileName2 = "C:/Users/Ross/Documents/Visual Studio 2015/Projects/Test/src/1.bmp";
string fileName3 = "C:/Users/Ross/Documents/Visual Studio 2015/Projects/Test/src/4.bmp";

string fileName4 = "C:/Users/Ross/OneDrive/Documents/PNGfingerprint/PNG/1_1.png";

int main( int argc, const char* argv[] )
{

	cv::Mat sourceImage = cv::imread(fileName4);

	namedWindow("Original Image", WINDOW_AUTOSIZE);  // Create a window for display.
	imshow("Original Image", sourceImage);           // Show our image inside it.

	cvtColor(sourceImage, sourceImage, CV_BGR2GRAY);

	Mat img = sourceImage.clone();
	Mat tmp, dst;
	IplImage ipl = sourceImage;

	cv::Mat m = cv::cvarrToMat(&ipl);  // default additional arguments: don't copy data.

	cvSmooth(&ipl, &ipl, CV_GAUSSIAN, 9, 9, 0);

	cv::threshold(m, dst, 50, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	namedWindow("Binarized2", CV_WINDOW_AUTOSIZE); //create a window with the name "Binarized"
	imshow("Binarized2", dst);
	//cv::threshold(img, img, 50, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	CvMat cvmat = m;

	cvAdaptiveThreshold(&cvmat, &cvmat, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 13, 1);
    localThreshold::binarisation(img, 41, 56);
    Mat binImg = img.clone();
    ideka::binOptimisation(img);

    namedWindow( "Binarised Image", WINDOW_AUTOSIZE );   // Create a window for display.
    imshow( "Binarised Image", binImg );                 // Show our image inside it.
    namedWindow( "Optimised Image", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Optimised Image", img );                  // Show our image inside it.

											   // Create markers image
	
	/*cv::Mat markers(m.size(), CV_8U, cv::Scalar(-1));
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
	cv::Mat wshedMask = segmenter.process(sourceImage);
	cv::Mat mask;
	convertScaleAbs(wshedMask, mask, 1, 0);
	double thresh = threshold(mask, mask, 1, 255, THRESH_BINARY);
	bitwise_and(m, m, dest1, mask);
	dest1.convertTo(dest1, CV_8U);

	imshow("final_result", dest1);*/



    //skeletionizing
    cv::bitwise_not(img, img);    //Inverse for bit-operations
    GuoHall::thinning(img);
    cv::bitwise_not(img, img);
    namedWindow( "Skeletenised Image", WINDOW_AUTOSIZE );   // Create a window for display.
    imshow( "Skeletenised Image", img );                    // Show our image inside it.

    //Minutiae-Extraction
    vector<Minutiae> minutiae;
    crossingNumber::getMinutiae(img, minutiae, 30);
    cout<<"Number of Minutiae Results : " << minutiae.size() << endl;

    //Visualisation
    Mat minutImg = img.clone();
    cvtColor(img, minutImg, CV_GRAY2RGB);
    for(std::vector<Minutiae>::size_type i = 0; i<minutiae.size(); i++){
        //add an transparent square at each minutiae-location
        int squareSize = 5;     //has to be uneven
        Mat roi = minutImg(Rect(minutiae[i].getLocX()-squareSize/2, minutiae[i].getLocY()-squareSize/2, squareSize, squareSize));
        double alpha = 0.3;
        if(minutiae[i].getType() == Minutiae::Type::RIDGEENDING){
            Mat color(roi.size(), CV_8UC3, cv::Scalar(255,0,0));    //blue square for ridgeending
            addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
        }else if(minutiae[i].getType() == Minutiae::Type::BIFURCATION){
            Mat color(roi.size(), CV_8UC3, cv::Scalar(0,0,255));    //red square for bifurcation
            addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
        }

    }
    namedWindow( "Minutie", WINDOW_AUTOSIZE );     // Create a window for display.
    imshow( "Minutie", minutImg );                 // Show our image inside it.

    //Minutiae-filtering
    Filter::filterMinutiae(minutiae);
    cout<<"Number minutiae after filtering : " << minutiae.size() << endl;

    Mat minutImg2 = img.clone();
    cvtColor(img, minutImg2, CV_GRAY2RGB);
    for(std::vector<Minutiae>::size_type i = 0; i<minutiae.size(); i++){
        //add an transparent square at each minutiae-location
        int squareSize = 5;     //has to be uneven
        Mat roi = minutImg2(Rect(minutiae[i].getLocX()-squareSize/2, minutiae[i].getLocY()-squareSize/2, squareSize, squareSize));
        double alpha = 0.3;
        if(minutiae[i].getType() == Minutiae::Type::RIDGEENDING){
            Mat color(roi.size(), CV_8UC3, cv::Scalar(255,0,0));    //blue square for ridgeending
            addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
        }else if(minutiae[i].getType() == Minutiae::Type::BIFURCATION){
            Mat color(roi.size(), CV_8UC3, cv::Scalar(0,0,255));    //red square for bifurcation
            addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
        }

    }
    namedWindow( "Filtered Minutiae", WINDOW_AUTOSIZE );     // Create a window for display.
    imshow( "Filtered Minutiae", minutImg2 );                 // Show our image inside it.
	time_t t = time(0); //get time now
	struct tm * now = localtime(&t);
	string time = std::to_string(now->tm_year) + 1900 + '-'
		+ std::to_string(now->tm_mon + 1) + '-'
		+ std::to_string(now->tm_mday);


	imwrite("foo2.png", minutImg2); //save minutiae with unique time for matching

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;

}
