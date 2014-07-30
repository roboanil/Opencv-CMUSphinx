#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "Cannot open web cam" << endl;
		return -1;
	}
	namedWindow("Source",CV_WINDOW_AUTOSIZE);
	namedWindow("Control",CV_WINDOW_NORMAL);
	namedWindow("Mod", CV_WINDOW_AUTOSIZE);
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout << "fps : " << fps << endl;
	
	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150;
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;

	int morph = 1;
	int morph2 = 1;

	int area = 10;

	createTrackbar("LowH", "Control", &iLowH, 179);
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255);
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);
	createTrackbar("HighV", "Control", &iHighV, 255);

	createTrackbar("ErodeDilate", "Control", &morph, 10);
	createTrackbar("DilateErode", "Control", &morph2, 10);
	
	createTrackbar("Area", "Control", &area, 1000);

	while (true)
	{
		Mat oriImage;
		
		bool imageSuccess = cap.read(oriImage);
		if (!imageSuccess)
		{
			cout << "Cannot read frame" << endl;
			break;
		}
		
		resize(oriImage, oriImage,Size(300,200));
		Mat box = Mat::zeros(oriImage.size(), CV_8UC3);
		imshow("Source", oriImage);
		/*
		Rect my(10, 10, 100, 100);
		oriImage = oriImage(my);
		*/
		Mat hsvImage;
		cvtColor(oriImage, hsvImage, COLOR_BGR2HSV);

		Mat thresholdedImage;
		inRange(hsvImage, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), thresholdedImage);

		if (morph > 0 && morph2 > 0){
			erode(thresholdedImage, thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(morph, morph)));
			dilate(thresholdedImage, thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(morph, morph)));

			dilate(thresholdedImage, thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(morph2, morph2)));
			erode(thresholdedImage, thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(morph2, morph2)));
		}
		/*
		Moments momentsBlob = moments(thresholdedImage);
		double m01 = momentsBlob.m01;
		double m10 = momentsBlob.m10;
		double areaBlob = momentsBlob.m00;

		if (areaBlob >= area){
			int centerX = m10 / areaBlob;
			int centerY = m01 / areaBlob;

			circle(box, Point(centerX, centerY), 5, cvScalar(255, 255, 0), 1);
			
		}
		oriImage = oriImage + box;
		imshow("Source", oriImage);
		*/
		imshow("Mod", thresholdedImage);
		
		char c = waitKey(30);
		if (c == 27) break;		
	}


	return 0;
	
}