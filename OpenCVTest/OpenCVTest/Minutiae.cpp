#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string.h>
#include <iostream> // includes C standard input / output definitions
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>

void filters(IplImage* image, int x, int y, double *xx);
IplImage *image = 0, *dstm = 0, *dstg = 0, *src = 0, *srcg = 0,
*srcr = 0, *dstav = 0, *dstsd = 0, *dstva = 0, *dstmax = 0, *dstmin = 0,
*dstmxmn = 0, *dst2d = 0;
char bufg[200], bufm[200], buft[200], buff[105];
char bufa[200], bufs[200], bufv[200], bufx[200], bufn[200],
bufxn[200], bufd[200];
IplImage* result;
// main programs
int main(int argc, char** argv)
{
	FILE *fpo, *fpg;
	char* filename = argc == 2 ? argv[1] : (char*)"thin.tif";
	//unsigned char av, sd, va, maax, min;
	int w = 1;

	if ((image = cvLoadImage(filename, 1)) == 0)
		return -1;
	IplImage* img = cvLoadImage("fp.jpg");
	int v_idx = 0;
	int pixel = 0;
	double cn;
	// convert to data or txt file
	if ((fpo = fopen("aaa.txt", "w")) == NULL)
	{
		printf("file not open for writing \n");
		exit(0);
	}
	if ((fpg = fopen("abb.txt", "w")) == NULL)
	{
		printf("file not open for writing \n");
		exit(0);
	}
	for (int j = 0; j<image->height; j = j + 1) {
		for (int i = 0; i<image->width; i = i + 1) {
			int v_idx = j*image->width + i;
			unsigned char pixel_value = image ->imageData[v_idx];
			fprintf(fpo, "%d %d %d\n", i, j, pixel_value);
			fprintf(fpo, "%d %d %d %d %d\n", x, y, red, grn, blu);
		}
	}
	result = cvLoadImage("thin.tif", 0);
	cvZero(result);
	CvScalar s;
	s.val[0] = 255;
	//minutiae extraction
	for (int y = 0; y<image->height; y = y + 3)
	{
		for (int x = 0; x<image->width; x = x + 3)
		{
			pc = (image->imageData[(j + 1)*image->width + (i + 1)]);
			int v_idx = y*image->width + x;
			filters(image, x, y, &cn);
			if (cn == 4 || cn == 5 || cn == 7) {
				cvSet2D(result, y + 1, x + 1, s);
				fprintf(fpo, "%d %d %lf\n", x + 1, y + 1, 255.000000);
			}
		}
	}
	cvSaveImage("result.tif", result);
	cvShowImage("fp.jpg", image);
	cvWaitKey(0);
	cvReleaseImage(&image);
	cvDestroyWindow("fp.jpg");
	return 0;
}
//window generator.
void filters(IplImage* image, int i, int j, double *cn)
{
	int x, y;
	unsigned char c1, c2, c3, c4, c5, c6, c7, c8;
	if (j<0 || j> image->height) { ; }
	else if (i<0 || i> image->width) { ; }
	else {
		c1 = (image->imageData[j*image->width + i]);
		c2 = (image->imageData[(j + 1)*image->width + i]);
		c3 = (image->imageData[(j + 2)*image->width + i]);
		c4 = (image->imageData[(j + 2)*image->width + (i + 1)]);
		c5 = (image->imageData[(j + 2)*image->width + (i + 2)]);
		c6 = (image->imageData[j*image->width + (i + 2)]);
		c7 = (image->imageData[j*image->width + (i + 2)]);
		c8 = (image->imageData[j*image->width + (i + 1)]);
		pc = (image->imageData[(j + 1)*image->width + (i + 1)]);
		c1 = abs(image->imageData[(j + 1)*image->width + i] -
			image->imageData[j*image->width + i]);;
		c2 = abs(image->imageData[(j + 2)*image->width + i] -
			image->imageData[(j + 1)*image->width + i]);
		c3 = abs(image->imageData[(j + 2)*image->width + (i + 1)]
			- image->imageData[(j + 2)*image->width + i]);//255;
		c4 = abs(image->imageData[(j + 2)*image->width + (i + 2)]
			- image->imageData[(j + 2)*image->width + (i + 1)]);//255;
		c5 = abs(image->imageData[(j + 1)*image->width + (i + 2)]
			- image->imageData[(j + 2)*image->width + (i + 2)]);//255;
		c6 = abs(image->imageData[j*image->width + (i + 2)] -
			image->imageData[(j + 1)*image->width + (i + 2)]);//255;
		c7 = abs(image->imageData[j*image->width + (i + 1)] -
			image->imageData[j*image->width + (i + 2)]);//255;
		c8 = abs(image->imageData[j*image->width + i] -
			image->imageData[j*image->width + (i + 1)]);//255;
		*cn = (double)(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8) / 255.0;
	}
}
}