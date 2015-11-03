#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "..\lib\gap-optimisation\GapOptimisation.h"

using namespace cv;
using namespace std;

int main( int argc, const char* argv[] ) {
	string fileName2 = "C:/Users/Ross/Documents/Visual Studio 2015/Projects/Test/src/1.bmp";
	cv::Mat sourceImage = cv::imread(fileName2, 0);

    namedWindow("Originales Bild", WINDOW_AUTOSIZE);
    imshow("Originales Bild", sourceImage);
    moveWindow("Originales Bild", 0, 0);

    Mat dirField = sourceImage.clone();
    GapOptimisation::drawDirectionField(dirField);

    namedWindow("Direction Field", WINDOW_AUTOSIZE);
    imshow("Direction Field", dirField);
    moveWindow("Direction Field", sourceImage.cols + 30, 0);


    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
