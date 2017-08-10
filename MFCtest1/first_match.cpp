
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>


#include "first_match.h"

////ROI set start point inside the calibration rectangle
//#define OFFSET 0
//#define PI 3.1415
//#ifndef MIN
//#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
//#endif
//
//#ifndef MAX
//#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
//#endif
using namespace cv;
using namespace std;







/*
calculate area
*/
int getArea(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
	
	double result;

	result = 0.5 * (x0 * y1
		- x1 * y0
		+ x1 * y2
		- x2 * y1
		+ x2 * y3
		- x3 * y2
		+ x3 * y0
		- x0 * y3);
	return result;

}

/*
input  cv::Point p1 p2
output double distance
*/
double getDistanceOfTwoCVPoints(Point p1, Point p2) {
	double dist;
	dist = sqrt( (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y) );
	return dist;
}






calibrationInfo AffineTrans(vector<Point2f> scrPoints, double physicalwidth, double physicalheight, Mat &color)
{
	Mat dst;
	
	Mat src = imread("C:\\Users\\高峰\\Documents\\denoised.jpg");
	//imshow("C:\\Users\\高峰\\Documents\\denoised1111111.jpg", src);

	calibrationInfo afterTrans;
	double area = 0.5*(scrPoints[0].x*scrPoints[1].y - scrPoints[1].x*scrPoints[0].y
		+ scrPoints[1].x*scrPoints[2].y - scrPoints[2].x*scrPoints[1].y
		+ scrPoints[2].x*scrPoints[3].y - scrPoints[3].x*scrPoints[2].y
		+ scrPoints[3].x*scrPoints[0].y - scrPoints[0].x*scrPoints[3].y);
	Point2f dstPoints[3];
	dstPoints[0].x = 0;
	dstPoints[0].y = 0;
	dstPoints[1].y = 0;
	dstPoints[2].x = 0;

	double k = sqrt(area / (physicalwidth*physicalheight));
	dstPoints[1].x = physicalwidth*k;
	dstPoints[2].y = physicalheight*k;
	//Point2f scrPointsa[4] = { scrPoints[0],scrPoints[1],scrPoints[2],scrPoints[3] };
	Point2f scrPointsa[3] = { scrPoints[0],scrPoints[1],scrPoints[3] };

	Mat Trans = getAffineTransform(scrPointsa, dstPoints);
	//warpAffine(color, dst, Trans, Size(color.cols, color.rows), CV_INTER_CUBIC);
	warpAffine(src, dst, Trans, Size(src.cols, src.rows));


	cv::imwrite("C:\\Users\\高峰\\Documents\\ready_to_recognize.jpg", dst);


	afterTrans.physicalLenth = physicalheight;
	afterTrans.physicalWidth = physicalwidth;
	afterTrans.lenth = dstPoints[1].x;
	afterTrans.width = dstPoints[2].y;
	return afterTrans;
}


/*
convert function
input in ROI result
output in cm (final answer)
瀹屾暣id璁＄畻鍦╮aw result鐢熸垚鏃跺畬鎴?鍦ㄦ纭
*/
finalResult rawresultToFinalResult(rawResult raw, calibrationInfo info) {

	//coefficient
	double coefficient;
	coefficient = info.lenth * info.width / (info.physicalLenth * info.physicalWidth);

	finalResult result;
	result.angle = (int) raw.angle;
	result.ifShape = raw.ifShape;
	result.colorID = raw.colorID;
	result.shapeID = raw.shapeID;
	//shapeCompleteID = 10*colorID + shapeID
	result.shapeComleteID = raw.colorID * 10 +raw.shapeID;
	result.objID = raw.objID;
	result.area = (int) (raw.area / coefficient);
	result.centreX = (int) ((raw.centreX + OFFSET) / (sqrt(coefficient)));
	result.centreY = (int) ((raw.centreY + OFFSET) / (sqrt(coefficient)));
	return result;
}



/**
* Helper function to find a cosine of angle between vectors
* from pt0->pt1 and pt0->pt2
*/
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/*
鑾峰彇棰滆壊
input RGB绌洪棿鍥惧儚
output int colorID
涓嶆敼鍙樿緭鍏?
*/
int getColor(const Mat& input, int x, int y) {
	
	
	Mat image = input.clone();
	Mat HSV;
	Mat RGB = image(Rect(x, y, 1, 1));
	cvtColor(RGB, HSV, CV_BGR2HSV);

	Vec3b hsv = HSV.at<Vec3b>(0, 0);

	int H = hsv.val[0];
	int S = hsv.val[1];
	int V = hsv.val[2];

	//榛戣壊 id 1
	if (V <= 46) return 1;

	//绾㈣壊 id 2
	else if (   (H<=10||H>=156)  &&  V>=46  &&  S>=43   )
	{
		return 2;
	}
	
	//榛勮壊 id 3
	else if (H >= 26 && H <= 34 && S >= 43 && V >= 46)
	{
		return 3;
	}

	//缁胯壊 id 4
	else if (H >= 35 && H <= 77 && S >= 43 && V >= 46)
	{
		return 4;
	}

	//钃濊壊 id 5
	else
	{
		return 5;
	}

}

/*
input 鏁扮粍鎸囬拡 rawResult[5]
	  input Mat
	  鐓х墖闇€瑕佷粠鏈湴璇诲彇
	  @@@ ROI set finished
output 鐩爣涓暟
*/

vector <rawResult> recognize(Mat& img_scene){
	vector <rawResult> result;

	//Mat img_scene = imread("C:\\Users\\濮氫箰瀹嘰\Desktop\\test3.jpg");
	//Mat img_scene = imread("D:\\璧勬枡\\yly\\myProject\\opencv_test_2.4.11\\test_project_2.4.13.2\\basic-shapes-2.png");
	
	



	// Convert to grayscale
	cv::Mat gray;
	cv::cvtColor(img_scene, gray, CV_BGR2GRAY);

	// Use Canny instead of threshold to catch squares with gradient shading
	cv::Mat bw;
	cv::Canny(gray, bw, 0, 50, 5);

	// Find contours
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<cv::Point> approx;
	cv::Mat dst = img_scene.clone();



	//閬嶅巻鍚刢ontours
	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		/*

		涓嶉渶瑕佷笁瑙掑舰

		if (approx.size() == 3)
		{
			setLabel(dst, "TRI", contours[i]);    // Triangles
		}
		*/

		//if (approx.size() >= 4 && approx.size() <= 6)
		if (approx.size() == 4)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc + 1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3) {

				//setLabel(dst, "RECT", contours[i]);

				//纭涓虹煩褰?
				//涓嬮潰杩涜璁＄畻

				rawResult singleResult;

				double height, width;
				double L1, L2, L3, L0;
				L0 = getDistanceOfTwoCVPoints(approx[0], approx[1]);
				L1 = getDistanceOfTwoCVPoints(approx[1], approx[2]);
				L2 = getDistanceOfTwoCVPoints(approx[2], approx[3]);
				L3 = getDistanceOfTwoCVPoints(approx[3], approx[0]);

				height = sqrt(L0 * L2);
				width = sqrt(L1 * L3);

				singleResult.area = height * width;


				//纭闀跨煭杞?
				//璁＄畻瑙掑害鍙栫偣
				Point p1, p2;
				double major, minor;

				if (width >= height) {
					major = width;
					minor = height;
					p1 = approx[0];
					p2 = approx[1];
				}
				else
				{
					major = height;
					minor = width;
					p1 = approx[1];
					p2 = approx[2];
				}

				//杈撳嚭瑙掑害
				singleResult.angle=atan2((p1.y-p2.y),(p1.x-p2.x)) * 180 / PI;

				//杈撳嚭centre
				singleResult.centreX = (approx[0].x+ approx[1].x+ approx[2].x+ approx[3].x) / 4;
				singleResult.centreY = (approx[0].y + approx[1].y + approx[2].y + approx[3].y) / 4;


				//褰㈢姸ID
				if (minor / major < 0.85) singleResult.shapeID = 3;
				else singleResult.shapeID = 2;

				singleResult.ifShape = true;

				singleResult.colorID = getColor(img_scene, singleResult.centreX, singleResult.centreY);

				singleResult.shapeComleteID = 10 * singleResult.colorID + singleResult.shapeID;

				result.push_back(singleResult);

			}

			/*
			
			涓嶉渶瑕?銆?杈瑰舰

			else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
				setLabel(dst, "PENTA", contours[i]);
			else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
				setLabel(dst, "HEXA", contours[i]);

			*/
		}
		else if(approx.size() >= 7)
		{
			
			Mat pointsf;
			Mat(contours[i]).convertTo(pointsf, CV_32F);
			RotatedRect box = fitEllipse(pointsf);

			//cv::Rect r = cv::boundingRect(contours[i]);

			if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 30)
				continue;

			//纭涓烘き鍦?
			//寮€濮嬪鐞?
			rawResult singleResult;

			double major, minor;
			major = MAX(box.size.width, box.size.height);
			minor = MIN(box.size.width, box.size.height);


			//centre
			singleResult.centreX = box.center.x;
			singleResult.centreY = box.center.y;


			//angle 
			singleResult.angle = box.angle - (double) 180;
			singleResult.colorID = getColor(img_scene, singleResult.centreX, singleResult.centreY);

			//褰㈢姸ID
			if (minor / major < 0.85) singleResult.shapeID = 4;
			else singleResult.shapeID = 1;

			singleResult.shapeComleteID = 10 * singleResult.colorID + singleResult.shapeID;

			singleResult.ifShape = true;
			//ares
			singleResult.area =(double)  (box.size.area() * PI / 4);
		//	寰呭畬鎴?
			//results[numOfTargets].centreX=(int) ((r.br+++)/4)

			/*
			
			涓鸿瘑鍒渾鍜屾き鍦嗚繘琛屼簡淇敼

			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
				setLabel(dst, "CIR", contours[i]);


			*/
			//鏇存柊鐩爣鏁伴噺
			result.push_back(singleResult);

		}
	}

	return result;
}

Mat denoised()
{
	const int N = 14;
	VideoCapture capture(0);
	int delay = 1000 / 30;

	//bool i = capture.isOpened();

	cv::Mat avrg_img;

	for (int i = 0; i < N; i++) {
		Mat frame;
		capture >> frame;
		// convert to double
		frame.convertTo(frame, CV_32F, 1.0 / 255.0);
		if (i == 0) {
			avrg_img = frame / N;
			//imshow("1", avrg_img);
		}
		else
			avrg_img += frame / N;
	}
	
	avrg_img.convertTo(avrg_img, CV_8UC3, 255.0);
	cv::imwrite("C:\\Users\\高峰\\Documents\\denoised.jpg", avrg_img);


	return avrg_img;
}


calibrationInfo cali;

