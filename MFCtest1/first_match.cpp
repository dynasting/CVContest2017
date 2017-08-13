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
#include <vector>
#include <opencv2/nonfree/nonfree.hpp>

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
	calibrationInfo afterTrans;
	double area = 0.5*(scrPoints[0].x*scrPoints[1].y - scrPoints[1].x*scrPoints[0].y
		+ scrPoints[1].x*scrPoints[2].y - scrPoints[2].x*scrPoints[1].y
		+ scrPoints[2].x*scrPoints[3].y - scrPoints[3].x*scrPoints[2].y
		+ scrPoints[3].x*scrPoints[0].y - scrPoints[0].x*scrPoints[3].y);
	Point2f dstPoints[4];
	dstPoints[0].x = 0;
	dstPoints[0].y = 0;
	dstPoints[2].y = 0;
	dstPoints[1].x = 0;
	double k = sqrt(area / (physicalwidth*physicalheight));
	k = k / 4;

	/*dstPoints[2].x = (int) (physicalwidth*k);
	dstPoints[1].y =(int) (physicalheight*k);
	dstPoints[3].x= (int) (physicalwidth*k);
	dstPoints[3].y= (int) (physicalheight*k);*/
	dstPoints[2].x = (int)480;
	dstPoints[1].y = (int)480;
	dstPoints[3].x = (int)480;
	dstPoints[3].y = (int)480;
	Point2f scrPointsa[4] = { scrPoints[0],scrPoints[3],scrPoints[1],scrPoints[2] };
	//Mat Trans = getAffineTransform(scrPointsa, dstPoints);
	Mat Trans = getPerspectiveTransform(scrPointsa, dstPoints);
	warpPerspective(color, dst, Trans, Size(color.cols, color.rows));
	//warpAffine(color, dst, Trans, Size(color.cols, color.rows), CV_INTER_CUBIC);


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
完整id计算在raw result生成时完成 在此确认
*/
finalResult rawresultToFinalResult(rawResult raw, calibrationInfo info) {

	//coefficient
	double coefficient;
	coefficient = 480 / info.physicalLenth;

	finalResult result;
	result.angle = (int)raw.angle;
	result.ifShape = raw.ifShape;
	result.colorID = raw.colorID;
	result.shapeID = raw.shapeID;
	//shapeCompleteID = 10*colorID + shapeID
	result.shapeComleteID = raw.colorID * 10 + raw.shapeID;
	result.objID = raw.objID;
	result.area = (int)(raw.area / coefficient / coefficient);
	result.centreX = (int)((raw.centreX + OFFSET) / (coefficient));
	result.centreY = (int)((raw.centreY + OFFSET) / (coefficient));
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



Mat denoised()
{
	const int N = 14;
	VideoCapture capture(0);
	int delay = 1000 / 30;

	cv::Mat avrg_img;

	for (int i = 0; i < N; i++) {
		Mat frame;
		capture >> frame;
		// convert to double
		frame.convertTo(frame, CV_32F, 1.0 / 255.0);
		if (i == 0) {
			avrg_img = frame / N;
		}
		else
			avrg_img += frame / N;
	}
	
	avrg_img.convertTo(avrg_img, CV_8UC3, 255.0);
	//cv::imwrite("C:\\Users\\高峰\\Desktop\\比赛图像\\测试图片\\小图\\denoised.jpg", avrg_img);
	return avrg_img;
}

calibrationInfo cali;


/**
* @num 0=tin 1=noodle 2=gum 3=popcan(hard)
* @逐一对比
@objID	 可乐 口香糖 方便面 饼干 81 82 83 84
* input id roi mat color
output rawResult
*/

//应使用新代码
//下次进行修改
//应设置return 特殊值 以防找不到实物
rawResult object_recognization_single(int object_num, Mat& image2)
{

	std::string filename[4] = { "D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\tin.jpg" ,
		"D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\noodles.JPG" ,
		"D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\gum.jpg" ,
		"D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\popcan.jpg" };
	Mat image1 = imread(filename[object_num], 1);

	
	
	//Mat img_scene = imread("D:\\资料\\yly\\myProject\\opencv_test_2.4.11\\Release\\stuff.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//int blockSize = 25;
	//int constValue = 10;
	//cv::Mat local;
	//cv::adaptiveThreshold(img_object0, img_object, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	//cv::adaptiveThreshold(img_scene0, img_scene, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);

	Ptr<FeatureDetector> detector;
	Ptr<DescriptorExtractor> extractor;

	initModule_nonfree();
	/*
	* SIFT,SURF, ORB
	*/
	detector = FeatureDetector::create("SIFT");
	extractor = DescriptorExtractor::create("SIFT");


	vector<KeyPoint> keypoints1, keypoints2;
	detector->detect(image1, keypoints1);
	detector->detect(image2, keypoints2);

	cout << "# keypoints of image1 :" << keypoints1.size() << endl;
	cout << "# keypoints of image2 :" << keypoints2.size() << endl;

	Mat descriptors1, descriptors2;
	extractor->compute(image1, keypoints1, descriptors1);
	extractor->compute(image2, keypoints2, descriptors2);



	cout << "Descriptors size :" << descriptors1.cols << ":" << descriptors1.rows << endl;

	vector< vector<DMatch> > matches12, matches21;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
	matcher->knnMatch(descriptors1, descriptors2, matches12, 2);
	matcher->knnMatch(descriptors2, descriptors1, matches21, 2);

	cout << "Matches1-2:" << matches12.size() << endl;
	cout << "Matches2-1:" << matches21.size() << endl;

	// ratio test proposed by David Lowe paper = 0.8
	std::vector<DMatch> good_matches1, good_matches2;

	for (int i = 0; i < matches12.size(); i++) {
		//const float ratio = 0.8;
		const float ratio = 0.9;

		if (matches12[i][0].distance < ratio * matches12[i][1].distance)
			good_matches1.push_back(matches12[i][0]);
	}

	for (int i = 0; i < matches21.size(); i++) {
		const float ratio = 0.9;
		//const float ratio = 0.8;
		if (matches21[i][0].distance < ratio * matches21[i][1].distance)
			good_matches2.push_back(matches21[i][0]);
	}

	cout << "Good matches1:" << good_matches1.size() << endl;
	cout << "Good matches2:" << good_matches2.size() << endl;

	// Symmetric Test
	std::vector<DMatch> better_matches;
	for (int i = 0; i<good_matches1.size(); i++) {
		for (int j = 0; j<good_matches2.size(); j++) {
			if (good_matches1[i].queryIdx == good_matches2[j].trainIdx && good_matches2[j].queryIdx == good_matches1[i].trainIdx) {
				better_matches.push_back(DMatch(good_matches1[i].queryIdx, good_matches1[i].trainIdx, good_matches1[i].distance));
				break;
			}
		}
	}


	// show it on an image
	Mat output;
	drawMatches(image1, keypoints1, image2, keypoints2, better_matches, output);




	//加入定位
	//-- Localize the object from img_1 in img_2
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < better_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints1[better_matches[i].queryIdx].pt);
		scene.push_back(keypoints2[better_matches[i].trainIdx].pt);
	}

	//add error handler
	if (obj.size() < 5) {
		rawResult res;
		res.area = 1;
		return res;
		//no obj
	}
	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0); obj_corners[1] = Point(image1.cols, 0);
	obj_corners[2] = Point(image1.cols, image1.rows); obj_corners[3] = Point(0, image1.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);


	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	Point2f offset((float)image1.cols, 0);
	line(output, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4);
	line(output, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 255, 0), 4);
	line(output, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 255, 0), 4);
	line(output, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 255, 0), 4);



	int area0;
	area0 = 0.5 * (scene_corners[0].x * scene_corners[1].y
		- scene_corners[1].x * scene_corners[0].y
		+ scene_corners[1].x * scene_corners[2].y
		- scene_corners[2].x * scene_corners[1].y
		+ scene_corners[2].x * scene_corners[3].y
		- scene_corners[3].x * scene_corners[2].y
		+ scene_corners[3].x * scene_corners[0].y
		- scene_corners[0].x * scene_corners[3].y);
	//std::cout << area0 << std::endl;
	//std::cout << "area finished" << endl;

	//other calculations
	rawResult res;


	res.centreX = (double)((scene_corners[0].x + scene_corners[1].x + scene_corners[2].x + scene_corners[3].x) / 4);
	res.centreY = (double)((scene_corners[0].y + scene_corners[1].y + scene_corners[2].y + scene_corners[3].y) / 4);


	res.angle = atan2((scene_corners[1].y - scene_corners[0].y), (scene_corners[1].x - scene_corners[0].x)) * 180 / PI;

	res.area = area0;

	res.colorID = 0;
	res.ifShape = false;

	res.shapeID = 0;



	switch (object_num)
	{
	case 0:
		res.shapeComleteID = 84;
		break;
	case 1:
		res.shapeComleteID = 83;
		break;
	case 2:
		res.shapeComleteID = 82;
		break;
	case 3:
		res.shapeComleteID = 81;
		break;
	}



	return res;



}

vector<rawResult> recognize(Mat & img_scene)
{
	vector<rawResult> results;
	rawResult get[4];
	get[0] = object_recognization_single(0, img_scene);
	get[1] = object_recognization_single(1, img_scene);
	get[2] = object_recognization_single(2, img_scene);
	get[3] = object_recognization_single(3, img_scene);
	if (get[0].area > MIN_AREA_OF_TARGET) {

	
		results.push_back(get[0]);

	}

	if (get[1].area > MIN_AREA_OF_TARGET) {
		results.push_back(get[1]);
	}
	if (get[2].area > MIN_AREA_OF_TARGET) {
		results.push_back(get[2]);

	}
	if (get[3].area > MIN_AREA_OF_TARGET) {
		results.push_back(get[3]);
	}
	return results;

}




