#include "ImgTransfromer.h"

ImgTransformer::ImgTransformer()
{
	corners = { { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } };
	outputCorners = { { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } };
}
ImgTransformer::~ImgTransformer() 
{
}

void ImgTransformer::updateImg(cv::Mat img)
{
	input = img;
}

void ImgTransformer::findInputCorners()
{
	this->corners.clear();
	//structure to sort coordinate point, sort by _y then _x
	struct compare_xy
	{
		bool operator ()(const cv::Point a, const cv::Point b) const {
			return (a.y == b.y ? a.x < b.x : a.y < b.y);
		}
	};
	this->preProcess();
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	//get the countours of the image, store the corners point only
	findContours(pInput, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//largest contour area
	double largestArea = 0;
	//largest contour index
	int index = 0;
	//get the largest contours
	for (int i = 0; i < contours.size(); i++)
	{
		double a = contourArea(contours[i], false);
		if (a > largestArea)
		{
			largestArea = a;
			index = i;
		}
	}
	double epsilon = arcLength(contours[index], true);
	approxPolyDP(cv::Mat(contours[index]), contours[index], epsilon*0.05, true);
	//update corners only when excate 4 point is found
	if (contours[index].size() == 4)
	{
		//sort the point by Y then X
		stable_sort(contours[index].begin(), contours[index].end(), compare_xy());
		for (int j = 0; j < 4; j++)
		{
			corners.push_back((cv::Point2f)contours[index][j]);
			//draw circle on the input frame for observation 
			circle(input, contours[index][j], 3, (0, 0, 255), -1, 8);
		}
	}
	else this->corners = { cv::Point2f(0, 0),cv::Point2f(pInput.cols, 0) ,cv::Point2f(0, pInput.rows),cv::Point2f(pInput.cols, pInput.rows) };

}
void ImgTransformer::setOutputCorners(std::vector<cv::Point2f> points)
{
	this->outputCorners = points;
}
void ImgTransformer::updateAffineMatix()
{
	this->findInputCorners();
	std::vector<cv::Point2f> corners3 = { corners[0],corners[1],corners[2] };
	//cout << corners3;//ERROR...
	std::vector<cv::Point2f> outputCorners3{ outputCorners[0],outputCorners[1],outputCorners[2] };; \
		affineMatrix = getAffineTransform(corners3, outputCorners3);
}
void ImgTransformer::updatePerspectiveMatix()
{
	this->findInputCorners();
	perspectiveMatrix = getPerspectiveTransform(corners, outputCorners);
}
cv::Mat ImgTransformer::affineTransfrom()
{
	warpAffine(input, output, affineMatrix, output.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
	return output;
}
cv::Mat ImgTransformer::perspectiveTransfrom()
{
	warpPerspective(input, output, perspectiveMatrix, output.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
	return output;
}
void ImgTransformer::preProcess()
{
	cvtColor(input, pInput, cv::COLOR_BGR2GRAY);
	blur(pInput, pInput, cv::Size(5, 5));
	threshold(pInput, pInput, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("process", pInput);
}
float ImgTransformer::getRation(float realWorldScale)
{
	int outPutImgScale = this->outputCorners[0].x - this->outputCorners[1].x;
	float ratio = (float)outPutImgScale / realWorldScale;
	return ratio;
}
