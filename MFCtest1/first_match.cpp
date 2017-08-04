/**
http://blog.sina.com.cn/s/blog_58649eb30100th2k.html
*/
//除了可乐都可以
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>

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



/*
convert function
input in ROI result
output in cm (final answer)
完整id计算在raw result生成时完成 在此确认
*/
finalResult rawresultToFinalResult(rawResult raw, calibrationInfo info) {

	//coefficient
	double coefficient;
	coefficient = getArea(info.xBottomLeft, info.yBottomLeft, info.xBottomRight, info.yBottomRight,
		info.xTopLeft, info.yTopLeft, info.xTopRight, info.yTopRight) / (info.physicalLenth * info.physicalWidth);

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
获取颜色
input RGB空间图像
output int colorID
不改变输入
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

	//黑色 id 1
	if (V <= 46) return 1;

	//红色 id 2
	else if (   (H<=10||H>=156)  &&  V>=46  &&  S>=43   )
	{
		return 2;
	}
	
	//黄色 id 3
	else if (H >= 26 && H <= 34 && S >= 43 && V >= 46)
	{
		return 3;
	}

	//绿色 id 4
	else if (H >= 35 && H <= 77 && S >= 43 && V >= 46)
	{
		return 4;
	}

	//蓝色 id 5
	else
	{
		return 5;
	}

}

/*
input 数组指针 rawResult[5]
	  input Mat
	  @@@ ROI set finished
output 目标个数
*/
int main() {
	
	int numOfTargets = 0;
	Mat img_scene = imread("C:\\Users\\姚乐宇\\Desktop\\test3.jpg");
	//Mat img_scene = imread("D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\test_project_2.4.13.2\\basic-shapes-2.png");
	rawResult results[12];
	calibrationInfo cali;
	cali.physicalLenth = 320;
	cali.physicalWidth = 320;
	cali.xBottomLeft = 0;
	cali.xBottomRight = 320;
	cali.xTopLeft = 0;
	cali.xTopRight = 320;

	cali.yBottomLeft = 320;
	cali.yBottomRight = 320;
	cali.yTopLeft = 0;
	cali.yTopRight = 0;




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



	//遍历各contours
	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		/*

		不需要三角形

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

				//确认为矩形
				//下面进行计算
				double height, width;
				double L1, L2, L3, L0;
				L0 = getDistanceOfTwoCVPoints(approx[0], approx[1]);
				L1 = getDistanceOfTwoCVPoints(approx[1], approx[2]);
				L2 = getDistanceOfTwoCVPoints(approx[2], approx[3]);
				L3 = getDistanceOfTwoCVPoints(approx[3], approx[0]);

				height = sqrt(L0 * L2);
				width = sqrt(L1 * L3);

				results[numOfTargets].area = height * width;


				//确认长短轴
				//计算角度取点
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

				//输出角度
				results[numOfTargets].angle=atan2((p1.y-p2.y),(p1.x-p2.x)) * 180 / PI;

				//输出centre
				results[numOfTargets].centreX = (approx[0].x+ approx[1].x+ approx[2].x+ approx[3].x) / 4;
				results[numOfTargets].centreY = (approx[0].y + approx[1].y + approx[2].y + approx[3].y) / 4;


				//形状ID
				if (minor / major < 0.85) results[numOfTargets].shapeID = 3;
				else results[numOfTargets].shapeID = 2;

				results[numOfTargets].ifShape = true;

				results[numOfTargets].colorID = getColor(img_scene, results[numOfTargets].centreX, results[numOfTargets].centreY);

				results[numOfTargets].shapeComleteID = 10 * results[numOfTargets].colorID + results[numOfTargets].shapeID;

				//更新目标数量
				numOfTargets++;
			}

			/*
			
			不需要5、6边形

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

			//确认为椭圆
			//开始处理


			double major, minor;
			major = MAX(box.size.width, box.size.height);
			minor = MIN(box.size.width, box.size.height);


			//centre
			results[numOfTargets].centreX = box.center.x;
			results[numOfTargets].centreY = box.center.y;


			//angle 
			results[numOfTargets].angle = box.angle - (double) 180;
			results[numOfTargets].colorID = getColor(img_scene, results[numOfTargets].centreX, results[numOfTargets].centreY);

			//形状ID
			if (minor / major < 0.85) results[numOfTargets].shapeID = 4;
			else results[numOfTargets].shapeID = 1;

			results[numOfTargets].shapeComleteID = 10 * results[numOfTargets].colorID + results[numOfTargets].shapeID;

			results[numOfTargets].ifShape = true;
			//ares
			results[numOfTargets].area =(double)  (box.size.area() * PI / 4);
		//	待完成
			//results[numOfTargets].centreX=(int) ((r.br+++)/4)

			/*
			
			为识别圆和椭圆进行了修改

			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
				setLabel(dst, "CIR", contours[i]);


			*/
			//更新目标数量
			numOfTargets++;
		}
	}
	for (int i = 0; i < numOfTargets; i++) {
		cout << results[i].shapeComleteID <<" "<< results[i].centreX << " " << results[i].centreY << endl;
	}
	return 0;
}







/**
* @num 0=tin 1=noodle 2=gum 3=popcan(hard)
* @逐一对比
@objID	 可乐 口香糖 方便面 饼干 81 82 83 84
* @return area in int
*/
int object_recognization_single(int object_num, Mat& img_scene)
{

	std::string filename[4] = { "D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\tin.jpg" ,
		"D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\noodles.JPG" ,
		"D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\gum.jpg" ,
		"D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\popcan.jpg" };
	Mat img_object = imread(filename[object_num], CV_LOAD_IMAGE_GRAYSCALE);
	//Mat img_scene = imread("D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\stuff.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//int blockSize = 25;
	//int constValue = 10;
	//cv::Mat local;
	//cv::adaptiveThreshold(img_object0, img_object, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	//cv::adaptiveThreshold(img_scene0, img_scene, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 350;

	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//printf("-- Max dist : %f \n", max_dist);
	//printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 4 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


	//-- Localize the object from img_1 in img_2
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0); obj_corners[1] = Point(img_object.cols, 0);
	obj_corners[2] = Point(img_object.cols, img_object.rows); obj_corners[3] = Point(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);


	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	//	Point2f offset((float)img_object.cols, 0);
	//line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 255, 0), 4);

	//-- Show detected matches
	//imshow("Good Matches & Object detection", img_matches);

	int area0;
	area0 = 0.5 * (scene_corners[0].x * scene_corners[1].y
		- scene_corners[1].x * scene_corners[0].y
		+ scene_corners[1].x * scene_corners[2].y
		- scene_corners[2].x * scene_corners[1].y
		+ scene_corners[2].x * scene_corners[3].y
		- scene_corners[3].x * scene_corners[2].y
		+ scene_corners[3].x * scene_corners[0].y
		- scene_corners[0].x * scene_corners[3].y);
	std::cout << area0 << std::endl;
	std::cout << "area finished" << endl;

	//other calculations
	return area0;


}

/**
* @收到图片后对比
* @brief Main function
* @num 0=tin 1=noodle 2=gum 3=popcan(hard)
*/
int object_recognization_single_mode(Mat& img_scene) {

	int area[4];
	for (int i = 0; i < 4; i++) {
		area[i] = object_recognization_single(i, img_scene);
	}

	int max_index = 0;
	int max_area = 0;
	for (int i = 0; i < 4; i++) {
		if (area[i] >= max_area) {
			max_index = i;
			max_area = area[i];
		}

	}


	//other output
	std::cout << max_index << std::endl;
	return max_index;

}

