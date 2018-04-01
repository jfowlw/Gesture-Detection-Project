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

void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst);

void myMotionEnergy(vector<Mat> mh, Mat& dst);

bool myTemplateMatching(Mat& mainFrame, Mat& testFrame, Mat& templt, Mat& results);
vector<Mat> createTemplates(Mat& image);

//global 
int frameSkipNum = 5;
Mat okay = imread("okay-hand.jpg", IMREAD_COLOR); // Read the file
Mat peace = imread("peacesign.jpg");
Mat wave1 = imread("wave1.jpg", IMREAD_COLOR);
Mat wave2 = imread("wave2.jpg", IMREAD_COLOR);
Mat wave3 = imread("wave3.jpg", IMREAD_COLOR);

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

	vector<Mat> templates = createTemplates(okay);
	vector<Mat> peaceTemp = createTemplates(peace);
	vector<Mat> wave1Temp = createTemplates(wave1);
	vector<Mat> wave2Temp = createTemplates(wave2);
	vector<Mat> wave3Temp = createTemplates(wave3);

	templates.insert(templates.end(), peaceTemp.begin(), peaceTemp.end());
	templates.insert(templates.end(), wave1Temp.begin(), wave1Temp.end());
	templates.insert(templates.end(), wave2Temp.begin(), wave2Temp.end());
	templates.insert(templates.end(), wave3Temp.begin(), wave3Temp.end());


	//imshow("templ", templ);
	//create a window called "MyVideo"
	namedWindow("MyVideo0", WINDOW_AUTOSIZE);
	RNG rng(12345);
	//imshow("hand shape",okay);

	// read a backgroundframe from video
	Mat frame, background;
	bool bSuccess = cap.read(frame);

	//if not successful, break loop
	if (!bSuccess)
	{
		cout << "Cannot read a frame from video stream" << endl;
	}
	//----------------
	// a) Read a stream of images from a web camera, and displaying the video
	//----------------


	//----------------
	//	b) Skin color detection
	//----------------
	// set background frame
	background = frame;
	int waveCount = 0;
	int skippedFrames = 0;

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

		skippedFrames = (skippedFrames+1)%frameSkipNum;


		// destination frame
		Mat frameDest, diff;
		diff = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		myFrameDifferencing(background, frame, diff);
		//imshow("background", background);
		imshow("diff", diff);

		Mat result;
		result = Mat::zeros(diff.rows, diff.cols, CV_8UC1);
		for (int i = 0; i < templates.size(); i++) {
			Mat templ = templates[i];
			if (myTemplateMatching(frame, diff, templ, result) == true) {
				if (i < 3) {
					cout << "okay" << endl;
					putText(frame, "OKAY", Point(frame.rows, 100), FONT_HERSHEY_PLAIN, 3, Scalar(0,255,0),2,8);
				}
				else {
					if (i < 6) {
						cout << "peace" << endl;
						putText(frame, "PEACE", Point(frame.rows, 100), FONT_HERSHEY_PLAIN, 3, Scalar(255, 0, 255), 2, 8);
					}
					else
					{
						cout << waveCount << endl;
						if (i < 9) {
							if (waveCount == 0)
							{
								//cout << "wave1" << endl;
								waveCount++;
							}
							else {
								putText(frame, "WAVE", Point(frame.rows, 100), FONT_HERSHEY_PLAIN, 3, Scalar(255, 255, 0), 2, 8);
							}
						}
						else {
							if (i < 12) {
								if (waveCount == 0)
								{
									//cout << "wave2" << endl;
									waveCount++;
								}
								else {
									putText(frame, "WAVE", Point(frame.rows, 100), FONT_HERSHEY_PLAIN, 3, Scalar(255, 255, 0), 2, 8);
								}
							}
							else {
							
								if (waveCount == 0)
								{
									//cout << "wave3" << endl;
									waveCount++;
								}
								else {
									putText(frame, "WAVE", Point(frame.rows, 100), FONT_HERSHEY_PLAIN, 3, Scalar(255, 255, 0), 2, 8);
								}
							}
						}
					}
				}
				break;
			}
			else {
				if (skippedFrames = 0)
				{
					waveCount = 0;
				}
			}
		}


		imshow("MyVideo0", frame);
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
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//For each pixel, compute the average intensity of the 3 color channels
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar (unsigned character)
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20) && (myMax(R, G, B) - myMin(R, G, B) > 15) && (abs(R - G) > 15) && (R > G) && (R > B)) {
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}


//Function that does frame differencing between the current frame and the previous frame
void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst) {
	absdiff(prev, curr, dst);
	Mat cpy = dst.clone();
	cvtColor(dst, cpy, COLOR_BGR2GRAY);
	dst =
		cpy > 50;
}


bool myTemplateMatching(Mat& mainFrame, Mat& testFrame, Mat& templt, Mat& results) {
	matchTemplate(testFrame, templt, results, CV_TM_CCOEFF_NORMED);
	//matchTemplate(frame, temp, result, CV_TM_SQDIFF_NORMED);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal, maxVal;
	Point  minLoc, maxLoc, matchLoc;

	minMaxLoc(results, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	if (maxVal > .65) {

		matchLoc = maxLoc;
		rectangle(mainFrame, matchLoc, Point(matchLoc.x + templt.cols, matchLoc.y + templt.rows), Scalar(0, 255, 0), 2, 8, 0);
		return true;
	}
	return false;
}


vector<Mat> createTemplates(Mat& image) {
	Mat temp, templ;
	vector<double> scales = { 1, 1.25, .75,.5}; //1.33, .66, 1.5, .5 
	//vector<double> scales = { .25 };
	//vector<double> scales = { .5 };
	//vector<double> scales = {.75};
	//vector<double> scales = { 1};
	//vector<double> scales = {1.25};
	//vector<double> scales = {1.5};
	//vector<double> scales = { 1.75 };
	//vector<double> scales = { 2 };



	vector<Mat> templates;
	for (double& scale : scales) {
		resize(image, temp, Size(), scale, scale);
		// destination image

		templ = Mat::zeros(temp.rows, temp.cols, CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
		mySkinDetect(temp, templ);
		templates.push_back(templ);
	}
	return templates;
}

//Function that accumulates the frame differences for a certain number of pairs of frames
void myMotionEnergy(vector<Mat> mh, Mat& dst) {
	Mat mh0 = mh[0];
	Mat mh1 = mh[1];
	Mat mh2 = mh[2];

	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			if (mh0.at<uchar>(i, j) == 255 || mh1.at<uchar>(i, j) == 255 || mh2.at<uchar>(i, j) == 255) {
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}