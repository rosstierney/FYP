#include "opencv2/opencv.hpp"

class Orientation {

public:
	cv::Mat orientation(cv::Mat inputImage)
	{
		cv::Mat orientationMat = cv::Mat::zeros(inputImage.size(), CV_8UC1);

		// compute gradients at each pixel
		cv::Mat grad_x, grad_y;
		cv::Sobel(inputImage, grad_x, CV_16SC1, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
		cv::Sobel(inputImage, grad_y, CV_16SC1, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);

		cv::Mat Vx, Vy, theta, lowPassX, lowPassY;
		cv::Mat lowPassX2, lowPassY2;

		Vx = cv::Mat::zeros(inputImage.size(), inputImage.type());
		Vx.copyTo(Vy);
		Vx.copyTo(theta);
		Vx.copyTo(lowPassX);
		Vx.copyTo(lowPassY);
		Vx.copyTo(lowPassX2);
		Vx.copyTo(lowPassY2);

		// estimate the local orientation of each block
		int blockSize = 16;

		for (int i = blockSize / 2; i < inputImage.rows - blockSize / 2; i += blockSize)
		{
			for (int j = blockSize / 2; j < inputImage.cols - blockSize / 2; j += blockSize)
			{
				float sum1 = 0.0;
				float sum2 = 0.0;

				for (int u = i - blockSize / 2; u < i + blockSize / 2; u++)
				{
					for (int v = j - blockSize / 2; v < j + blockSize / 2; v++)
					{
						sum1 += grad_x.at<float>(u, v) * grad_y.at<float>(u, v);
						sum2 += (grad_x.at<float>(u, v)*grad_x.at<float>(u, v)) * (grad_y.at<float>(u, v)*grad_y.at<float>(u, v));
					}
				}

				Vx.at<float>(i, j) = sum1;
				Vy.at<float>(i, j) = sum2;

				double calc = 0.0;

				if (sum1 != 0 && sum2 != 0)
				{
					calc = 0.5 * atan(Vy.at<float>(i, j) / Vx.at<float>(i, j));
				}

				theta.at<float>(i, j) = calc;

				// Perform low-pass filtering
				float angle = 2 * calc;
				lowPassX.at<float>(i, j) = cos(angle * CV_PI / 180);
				lowPassY.at<float>(i, j) = sin(angle * CV_PI / 180);

				float sum3 = 0.0;
				float sum4 = 0.0;
				int lowPassSize;

				for (int u = -lowPassSize / 2; u < lowPassSize / 2; u++)
				{
					for (int v = -lowPassSize / 2; v < lowPassSize / 2; v++)
					{
						sum3 += inputImage.at<float>(u, v) * lowPassX.at<float>(i - u*lowPassSize, j - v * lowPassSize);
						sum4 += inputImage.at<float>(u, v) * lowPassY.at<float>(i - u*lowPassSize, j - v * lowPassSize);
					}
				}
				lowPassX2.at<float>(i, j) = sum3;
				lowPassY2.at<float>(i, j) = sum4;

				float calc2 = 0.0;

				if (sum3 != 0 && sum4 != 0)
				{
					calc2 = 0.5 * atan(lowPassY2.at<float>(i, j) / lowPassX2.at<float>(i, j)) * 180 / CV_PI;
				}
				orientationMat.at<float>(i, j) = calc2;

			}

		}
		return orientationMat;
	}
};