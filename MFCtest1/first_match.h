#pragma once
#ifndef  FIRST_MATCH_H
#define FIRST_MATCH_H

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
//#include "first_match.cpp"

//ROI set start point inside the calibration rectangle
#define OFFSET 0
#define PI 3.1415

using namespace cv;
using namespace std;


/*
unit pix
bool ifshape
colorID  黑红黄绿蓝12345
shapeID  圆 正方形 长方形 椭圆 1234
objID	 可乐 口香糖 方便面 饼干 81 82 83 84
shapeCompleteID = 10*colorID + shapeID
double centreX.Y
double angle
double area
*/
struct rawResult
{
	bool ifShape;
	int colorID;//colorID  黑红黄绿蓝12345
	int shapeID;//shapeID  圆 正方形 长方形 椭圆 1234
	int objID;//objID	 可乐 口香糖 方便面 饼干 81 82 83 84
	int shapeComleteID;//shapeCompleteID = 10*colorID + shapeID
	double centreX, centreY;
	double angle;
	double area;
};

/*
final result for display
unit cm
colorID  黑红黄绿蓝12345
shapeID  圆 正方形 长方形 椭圆 1234
objID	 可乐 口香糖 方便面 饼干 81 82 83 84
shapeCompleteID = 10*colorID + shapeID
*/
struct finalResult
{
	bool ifShape;
	int colorID;
	int shapeID;
	int objID;
	int shapeComleteID;
	int centreX, centreY;
	int angle;
	int area;
};

/*
calculate area
*/
int getArea(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);


/*
input  cv::Point p1 p2
output double distance
*/
double getDistanceOfTwoCVPoints(Point p1, Point p2);

struct calibrationInfo
{
	double physicalLenth;
	double physicalWidth;
	int xTopLeft;
	int yTopLeft;
	int xTopRight;
	int yTopRight;
	int xBottomLeft;
	int yBottomLeft;
	int xBottomRight;
	int yBottomRight;
};

/*
convert function
input in ROI result
output in cm (final answer)
完整id计算在raw result生成时完成 在此确认
*/
finalResult rawresultToFinalResult(rawResult raw, calibrationInfo info);


/**
* Helper function to find a cosine of angle between vectors
* from pt0->pt1 and pt0->pt2
*/
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

/*
获取颜色
input RGB空间图像
output int colorID
不改变输入
*/
int getColor(const Mat& input, int x, int y);




#endif // ! FIRST_MATCH_H

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
