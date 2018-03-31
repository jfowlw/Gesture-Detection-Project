/*	CS440_640_Lab6_part2.cpp
*	Artificial Intelligence Spring 2018
*	Lab 6 part2
*	--------------
*	This program introduces the following concepts:
*		a) Read a stream of images from a web camera, and displaying the video
*		b) Skin color detection
*	--------------
*/

//opencv libraries
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//C++ standard libraries
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//Global variables
Mat img; Mat templ;
//function declarations

/**
Function that returns the maximum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMax(int a, int b, int c);

/**
Function that returns the minimum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMin(int a, int b, int c);

/**
Function that detects whether a pixel belongs to the skin based on RGB values
@param src The source color image
@param dst The destination grayscale image where skin pixels are colored white and the rest are colored black
*/
void mySkinDetect(Mat& src, Mat& dst);

int main()
{
	
	//----------------
	//a) Reading a stream of images from a webcamera, and displaying the video
	//----------------
	// For more information on reading and writing video: http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
	// open the video camera no. 0
	VideoCapture cap(0);

	// if not successful, exit program
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	Mat image, temp, result;
	image = imread("okay-hand.jpg", IMREAD_COLOR); // Read the file
	//image2 = imread("okay-hand3.jpg", IMREAD_COLOR);
	Size size(image.cols / 2, image.rows / 2);
	resize(image, temp, size);
	// destination image
	templ = Mat::zeros(temp.rows, temp.cols, CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
	//img = Mat::zeros(image2.rows, image2.cols, CV_8UC1);
	mySkinDetect(temp, templ);
	imshow("templ", templ);
	//create a window called "MyVideo"
	namedWindow("MyVideo0", WINDOW_AUTOSIZE);
	//create a window called "Skin" for skin color detection
	namedWindow("Skin", WINDOW_AUTOSIZE);

	namedWindow("Results", WINDOW_AUTOSIZE);

	while (1)
	{
		// read a new frame from video
		Mat frame;
		bool bSuccess = cap.read(frame);

		//if not successful, break loop
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		//----------------
		// a) Read a stream of images from a web camera, and displaying the video
		//----------------
		

		//----------------
		//	b) Skin color detection
		//----------------

		// destination frame
		Mat frameDest;
		frameDest = Mat::zeros(frame.rows, frame.cols, CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
		mySkinDetect(frame, frameDest);
		result = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		int erosion_size = 3;
		//Mat element = getStructuringElement(MORPH_RECT,
		//	Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		//	Point(erosion_size, erosion_size)
		//);

		//erode(frameDest, frameDest, element);
		//dilate(frameDest, frameDest, element);
		//dilate(frameDest, frameDest, element);


		matchTemplate(frameDest, templ, result, CV_TM_CCOEFF);
		//matchTemplate(frame, temp, result, CV_TM_SQDIFF_NORMED);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		double minVal, maxVal;
		Point  minLoc, maxLoc, matchLoc;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		cout << maxVal << endl;
		matchLoc = maxLoc;
		rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(200,200,0), 2, 8, 0);
		rectangle(frame, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
		imshow("MyVideo0", frame);
		imshow("Skin", frameDest);
		imshow("Results", result);
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	cap.release();
	return 0;
}

//Function that returns the maximum of 3 integers
int myMax(int a, int b, int c) {
	int m = a;
	(void)((m < b) && (m = b));
	(void)((m < c) && (m = c));
	return m;
}

//Function that returns the minimum of 3 integers
int myMin(int a, int b, int c) {
	int m = a;
	(void)((m > b) && (m = b));
	(void)((m > c) && (m = c));
	return m;
}

//Function that detects whether a pixel belongs to the skin based on RGB values
void mySkinDetect(Mat& src, Mat& dst) {
	//Surveys of skin color modeling and detection techniques:
	//Vezhnevets, Vladimir, Vassili Sazonov, and Alla Andreeva. "A survey on pixel-based skin color detection techniques." Proc. Graphicon. Vol. 3. 2003.
	//Kakumanu, Praveen, Sokratis Makrogiannis, and Nikolaos Bourbakis. "A survey of skin-color modeling and detection methods." Pattern recognition 40.3 (2007): 1106-1122.
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			//For each pixel, compute the average intensity of the 3 color channels
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar (unsigned character)
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20) && (myMax(R, G, B) - myMin(R, G, B) > 15) && (abs(R - G) > 15) && (R > G) && (R > B)){
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}
