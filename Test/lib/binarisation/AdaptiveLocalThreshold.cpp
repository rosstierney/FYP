#include "AdaptiveLocalThreshold.h"

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace cv;

namespace localThreshold {

    void binarisation(cv::Mat& im, int numTilesWidth, int numTilesHeight){
        int deltaThreshold = 50;
        int tileWidth = im.cols/numTilesWidth;
        int tileHeight = im.rows/numTilesHeight;
        unsigned int greyValuesAdded = 0;
        unsigned int pixelPerTile = tileWidth * tileHeight;
        unsigned char intensity = 0;
        int numOfTiles = numTilesWidth*numTilesHeight;
        std::vector<unsigned int> thresholds (numOfTiles+numTilesWidth+numTilesHeight+1);  //Added edge to areas
		
		std::vector<unsigned char> deltas (numOfTiles+numTilesWidth+numTilesHeight+1);
        int i, j, k, l, t;
        t = 0;
        //Laplace
        /*int kernel_size = 3;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;
        GaussianBlur( im, im, Size(3,3), 0, 0, BORDER_DEFAULT );
        Laplacian( im, im, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( im, im );*/
        //Iterate through the image per step, a Tile analyzed
        for(i = 0; i<=im.cols-tileWidth; i=i+tileWidth){
            for(j = 0; j<=im.rows-tileHeight; j=j+tileHeight){

                unsigned char darkest = 255;
                unsigned char brightest = 0;

                //Iteration by Tile
                for(k = i; ((k-i)<tileWidth); k++){
                    for(l = j; ((l-j)<tileHeight); l++){
                        //add up all the gray values
                        intensity = im.at<uchar>(cv::Point(k, l));
                        greyValuesAdded += intensity;

                        //Calculate maximum and minimum values
                        if(intensity < darkest){
                            darkest = intensity;
                        }else if(intensity>brightest){
                            brightest = intensity;
                        }

                    }
                }
                //Deltas speichern
                deltas[t] = std::abs(brightest-darkest);
                //std::cout << "delta = " << std::abs(brightest-darkest) << std::endl;
                //Druchschnitt der Grauwerte speichern
                thresholds[t] = greyValuesAdded/pixelPerTile;
                //std::cout << "t= " << t << std::endl;
                greyValuesAdded = 0;
                t++;
            }
        }

        int numOfThresholds = t;

        //medianThresholding(thresholds, numOfThresholds);
        //averageThresholding(thresholds, numOfThresholds);

		//convert vector to array
		std::vector<unsigned int> v = thresholds;
		unsigned int* a = &v[0];
		std::vector<unsigned char> v1 = deltas;
		unsigned char* d = &v1[0];
        deltaThresholding(a, numOfThresholds, d, deltaThreshold);

        t = 0;
        //2. Iterate through the image step by step, and process tile
        for(i = 0; i<=im.cols-tileWidth; i=i+tileWidth){
            for(j = 0; j<=im.rows-tileHeight; j=j+tileHeight){

                //Threshold applicable to Tile
                for(k = i; ((k-i)<tileWidth); k++){
                    for(l = j; ((l-j)<tileHeight); l++){
                        intensity = im.at<uchar>(cv::Point(k, l));
                        if(intensity >= thresholds[t]){
                            im.at<uchar>(l,k) = 255; //white
                        }else{
                            im.at<uchar>(l,k) = 0; //black
                        }

                    }
                }
                t++;
            }
        }
    }

    void deltaThresholding(unsigned int thresholds[], int numOfThresholds, unsigned char deltas[], int deltaThreshold){
        for(int i = 0; i<numOfThresholds; i++){
            if(deltas[i]<deltaThreshold){
                thresholds[i] = 0;
            }
        }
    }

    void medianThresholding(unsigned int thresholds[], int numOfThresholds){
        std::vector<int> sortedThresholds(numOfThresholds);
		std::vector<int> v2 = sortedThresholds;
		int* b = &v2[0];

        std::copy(thresholds, thresholds + numOfThresholds, b);
		
        std::sort(b, b + numOfThresholds);
        unsigned int thresholdThreshold = sortedThresholds[numOfThresholds/2];
        //Thresholds bearbeiten
        for(int i = 0; i<numOfThresholds; i++){
            if(thresholds[i]>thresholdThreshold){
                thresholds[i] = 0;
            }
        }
    }

    void averageThresholding(unsigned int thresholds[], int numOfThresholds){
        unsigned int thresholdValues = 0;
        unsigned int thresholdThreshold = 0;

        //Thresholds analysieren
        for(int i = 0; i<numOfThresholds; i++){
            thresholdValues += thresholds[i];
        }
        thresholdThreshold = thresholdValues/numOfThresholds; //Mittelwert der Thresholds
        //Thresholds bearbeiten
        for(int i = 0; i<numOfThresholds; i++){
            if(thresholds[i]>thresholdThreshold){
                thresholds[i] = 0;
            }
        }
    }
}
