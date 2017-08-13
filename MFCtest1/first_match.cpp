
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
����id������raw result����ʱ��� �ڴ�ȷ��
*/
finalResult rawresultToFinalResult(rawResult raw, calibrationInfo info) {

	//coefficient
	double coefficient;
	coefficient = 480 / info.physicalLenth ;

	finalResult result;
	result.angle = (int) raw.angle;
	result.ifShape = raw.ifShape;
	result.colorID = raw.colorID;
	result.shapeID = raw.shapeID;
	//shapeCompleteID = 10*colorID + shapeID
	result.shapeComleteID = raw.colorID * 10 +raw.shapeID;
	result.objID = raw.objID;
	result.area = (int) (raw.area / coefficient/ coefficient);
	result.centreX = (int) ((raw.centreX + OFFSET) / (coefficient));
	result.centreY = (int) ((raw.centreY + OFFSET) / (coefficient));
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
��ȡ��ɫ
input RGB�ռ�ͼ��
output int colorID
���ı�����
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

	//��ɫ id 1
	if (V <= 46) return 1;

	//��ɫ id 2
	else if (   (H<=10||H>=156)  &&  V>=46  &&  S>=43   )
	{
		return 2;
	}
	
	//��ɫ id 3
	else if (H >= 26 && H <= 34 && S >= 43 && V >= 46)
	{
		return 3;
	}

	//��ɫ id 4
	else if (H >= 35 && H <= 77 && S >= 43 && V >= 46)
	{
		return 4;
	}

	//��ɫ id 5
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
	//cv::imwrite("C:\\Users\\�߷�\\Desktop\\����ͼ��\\����ͼƬ\\Сͼ\\denoised.jpg", avrg_img);
	return avrg_img;
}

calibrationInfo cali;


/**
* @num 0=tin 1=noodle 2=gum 3=popcan(hard)
* @��һ�Ա�
@objID	 ���� ������ ������ ���� 81 82 83 84
* input id roi mat color
output rawResult
*/

//Ӧʹ���´���
//�´ν����޸�
//Ӧ����return ����ֵ �Է��Ҳ���ʵ��
rawResult object_recognization_single(int object_num, Mat& image2)
{

	std::string filename[4] = { "C:\\Users\\高峰\\Desktop\\比赛图像\\tin.jpg" ,
		"C:\\Users\\高峰\\Desktop\\比赛图像\\noodles.JPG" ,
		"C:\\Users\\高峰\\Desktop\\比赛图像\\gum.jpg" ,
		"C:\\Users\\高峰\\Desktop\\比赛图像\\popcan.jpg" };
	Mat image1 = imread(filename[object_num], 1);

	
	

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







	//���붨λ
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

vector<rawResult> recognizeObj(Mat & img_scene)
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

bool check_validation(rawResult shape, rawResult obj) {
	double dst;
	dst = getDistanceOfPoints(shape.centreX, shape.centreY, obj.centreX, obj.centreY);
	if (dst < sqrt(obj.area)) return false;
	else return true;
}


double getDistanceOfPoints(double x1, double y1, double x2, double y2) {
	double dist;
	dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	return dist;
}



vector <rawResult> recognizeShape(Mat& img_scene) {
	vector <rawResult> result;

	
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



	
	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;


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

			
				singleResult.angle = atan2((p1.y - p2.y), (p1.x - p2.x)) * 180 / PI;

				
				singleResult.centreX = (approx[0].x + approx[1].x + approx[2].x + approx[3].x) / 4;
				singleResult.centreY = (approx[0].y + approx[1].y + approx[2].y + approx[3].y) / 4;


				
				if (minor / major < 0.85) singleResult.shapeID = 3;
				else singleResult.shapeID = 2;

				singleResult.ifShape = true;

				singleResult.colorID = getColor(img_scene, singleResult.centreX, singleResult.centreY);

				singleResult.shapeComleteID = 10 * singleResult.colorID + singleResult.shapeID;

				result.push_back(singleResult);

			}

			
		}
		else if (approx.size() >= 7)
		{

			Mat pointsf;
			Mat(contours[i]).convertTo(pointsf, CV_32F);
			RotatedRect box = fitEllipse(pointsf);

			//cv::Rect r = cv::boundingRect(contours[i]);

			if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 30)
				continue;

			
			rawResult singleResult;

			double major, minor;
			major = MAX(box.size.width, box.size.height);
			minor = MIN(box.size.width, box.size.height);


			//centre
			singleResult.centreX = box.center.x;
			singleResult.centreY = box.center.y;


			//angle 
			singleResult.angle = box.angle - (double)180;
			singleResult.colorID = getColor(img_scene, singleResult.centreX, singleResult.centreY);

			
			if (minor / major < 0.85) singleResult.shapeID = 4;
			else singleResult.shapeID = 1;

			singleResult.shapeComleteID = 10 * singleResult.colorID + singleResult.shapeID;

			singleResult.ifShape = true;
			//ares
			singleResult.area = (double)(box.size.area() * PI / 4);
			
			result.push_back(singleResult);

		}
	}

	return result;
}