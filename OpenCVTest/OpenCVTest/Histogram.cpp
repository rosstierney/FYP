#include "Histogram.h";


Histogram::Histogram()
{

}

Mat Histogram::showHistogram(Mat & img)
{
	/*int bins = 256;             // number of bins
	int nc = img.channels();    // number of channels
	vector<Mat> hist(nc);       // array for storing the histograms
	vector<Mat> canvas(nc);     // images for displaying the histogram
	int hmax[3] = { 0,0,0 };      // peak value for each histogram

	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, bins, CV_32SC1);

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			for (int k = 0; k < nc; k++)
			{
				uchar val = nc == 1 ? img.at<uchar>(i, j) : img.at<Vec3b>(i, j)[k];
				hist[k].at<int>(val) += 1;
			}
		}
	}

	for (int i = 0; i < nc; i++)
	{
		for (int j = 0; j < bins - 1; j++)
			hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
	}

	const char* wname[3] = { "blue", "green", "red" };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

	for (int i = 0; i < nc; i++)
	{
		canvas[i] = Mat::ones(125, bins, CV_8UC3);

		for (int j = 0, rows = canvas[i].rows; j < bins - 1; j++)
		{
			line(
				canvas[i],
				Point(j, rows),
				Point(j, rows - (hist[i].at<int>(j) * rows / hmax[i])),
				nc == 1 ? Scalar(200, 200, 200) : colors[i],
				1, 8, 0
				);
		}

		imshow(nc == 1 ? "value" : wname[i], canvas[i]);
	}*/
	Mat output;
	cvtColor(img, img, CV_BGR2GRAY);
	equalizeHist(img, output);
	return output;
}
