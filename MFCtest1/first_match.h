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
#include <vector>

//#include "first_match.cpp"

//ROI set start point inside the calibration rectangle
#define OFFSET 5
#define PI 3.1415

using namespace cv;
using namespace std;


/*
unit pix
bool ifshape
colorID  �ں������12345
shapeID  Բ ������ ������ ��Բ 1234
objID	 ���� ������ ������ ���� 81 82 83 84
shapeCompleteID = 10*colorID + shapeID
double centreX.Y
double angle
double area
*/
struct rawResult
{
	bool ifShape;
	int colorID;//colorID  �ں������12345
	int shapeID;//shapeID  Բ ������ ������ ��Բ 1234
	int objID;//objID	 ���� ������ ������ ���� 81 82 83 84
	int shapeComleteID;//shapeCompleteID = 10*colorID + shapeID
	double centreX, centreY;
	double angle;
	double area;
};

/*
final result for display
unit cm
colorID  �ں������12345
shapeID  Բ ������ ������ ��Բ 1234
objID	 ���� ������ ������ ���� 81 82 83 84
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
	int lenth;
	int width;
	
};



//���α궨
//color Ϊ�����Ĳ�ɫͼ
//����double���Ա궨����
//���vector���Ա궨
//����ṹ�� ��physical��Ϊʵ�ʲ���������
//������Ϊ���ͼƬ�ĳ���
//����Ľ��imwrite()
//����ĵ��˳��Ӧ���ǣ����ϣ����ϣ����£�����
//Ť��ʱ������ӳ��������ǰ��������������ĸ�
//Ȼ���������Ч�����á������Կ��ǻ����任���Ǹ���Ҫ�ĸ���ġ���
calibrationInfo AffineTrans(vector <Point2f> scrPoints, double physicalwidth, double physicalheight,Mat& color);

//ͼƬ����



/*
convert function
input in ROI result
output in cm (final answer)
����id������raw result����ʱ��� �ڴ�ȷ��
*/
finalResult rawresultToFinalResult(rawResult raw, calibrationInfo info);


/**
* Helper function to find a cosine of angle between vectors
* from pt0->pt1 and pt0->pt2
*/
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

/*
��ȡ��ɫ
input RGB�ռ�ͼ��
output int colorID
���ı�����
*/
int getColor(const Mat& input, int x, int y);


/*
input ����ָ�� rawResult[5]
input Mat

@@@ ROI set finished
output Ŀ�����
*/
vector <rawResult> recognize(Mat& img_scene);



/*
����
����
*/
Mat denoised();

extern calibrationInfo cali;
#endif // ! FIRST_MATCH_H

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
