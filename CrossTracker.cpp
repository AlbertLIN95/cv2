#include "CrossTracker.h"



void CrossTracker::updateImg(cv::Mat img)
{
	object = img;
	this->size[0] = img.rows;
	this->size[1] = img.cols;
}
cv::Mat CrossTracker::preProcess(cv::Mat img)
{
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
	blur(img, img, cv::Size(5, 5));
	threshold(img, img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	return img;
}
CrossTracker::CrossTracker()
{
	this->width = 10;
	*location = (0, 0);
	angle = 0;
}
CrossTracker::~CrossTracker()                 // destructor, just an example
{
	delete[] location;
}
void CrossTracker::setROIWidth(int width)
{
	this->width = width;
}
void CrossTracker::showCross()
{
	//draw a point in the middle of the cross
	circle(object, cv::Point(location[0], location[1]), 3, (0, 0, 255), -1, 8);
	cv::imshow("object", object);
}
void CrossTracker::findCross()
{

	//4 ROI of the objective image
	cv::Mat lineImg[4];
	//offset of the lineImg
	cv::Point offset[4] = { cv::Point(0,0),cv::Point(0, size[0] - width) ,cv::Point(0, 0) ,cv::Point(size[1] - width, 0) };
	//size of lineImg
	cv::Point imgSize[4] = { cv::Point(size[1],width),cv::Point(size[1], width) ,cv::Point(width, size[0]) ,cv::Point(width, size[0]) };
	//contours of 4 ROI
	std::vector<std::vector<cv::Point>> contours[4];
	//center of mass of the largest contours
	cv::Point massCenter[4];
	//get 4 ROIs
	for (int i = 0; i < 4; i++)
	{
		lineImg[i] = preProcess(object(cv::Rect(offset[i].x, offset[i].y, imgSize[i].x, imgSize[i].y)));
		std::string a = "lineCam" + i;
		imshow(a, lineImg[i]);
		findContours(lineImg[i], contours[i], CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, offset[i]);
		double largestArea = 0;
		int largestContourIndex = 0;
		//get the largest contours and find its center of mass
		for (int j = 0; j < contours[i].size(); j++)//get the largest contours of 
		{
			double a = contourArea(contours[i][j], false);
			if (a > largestArea)
			{
				largestArea = a;
				largestContourIndex = j;
			}
		}
		cv::Moments m = moments(contours[i][largestContourIndex], false);
		massCenter[i].x = m.m10 / m.m00;
		massCenter[i].y = m.m01 / m.m00;
		circle(object, cv::Point(massCenter[i].x, massCenter[i].y), 3, (0, 255, 0), -1, 8);
	}
	this->angle = massCenter[0].x - massCenter[1].y;
	location[0] = (massCenter[0].x + massCenter[1].x) / 2;
	location[1] = (massCenter[2].y + massCenter[3].y) / 2;

}
int CrossTracker::getAngle() { return angle; }
int* CrossTracker::getLocation() { return location; }
int*  CrossTracker::getSize() { return size; }
