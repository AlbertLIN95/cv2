#include <opencv2\opencv.hpp>
#include <iostream>
#include "ImgTransfromer.h"
#include "CrossTracker.h"
#include "PID.h"

int main(int argc, char** argv)
{
	int* crossLocation;
	int crossAngle;
	cv::Mat frame, warpFrame;

	ImgTransformer* transform = new ImgTransformer();

	std::vector<cv::Point2f> outputSize{ cv::Point2f(0, 0),cv::Point2f(640, 0),cv::Point2f(0, 480),cv::Point2f(640, 480) };
	
	CrossTracker* tracker = new CrossTracker();
	tracker->setROIWidth(60);
	transform->setOutputCorners(outputSize);
	
	cv::VideoCapture cap(1);
	if (!cap.isOpened())
		return 0;
	while (cv::waitKey(30) != 'r')
	{
		cap >> frame;
		transform->updateImg(frame);
		/* Perstive transform
		transform->updatePerspectiveMatix();
		warpFrame=transform->PerspectiveTransfrom();
		imshow("aff", warpFrame);
		*/
		transform->updateAffineMatix();
		warpFrame = transform->affineTransfrom();
		imshow("perspect", warpFrame);
		imshow("frame", frame);
	}

	for (;;)
	{
		//get the top view of the cross
		cap >> frame;
		transform->updateImg(frame);
		warpFrame = transform->affineTransfrom();
		//imshow("affine", warpFrame);
		//find the cross location
		tracker->updateImg(warpFrame);
		tracker->findCross();
	
		crossAngle=tracker->getAngle();
		crossLocation = tracker->getLocation();

		//for debug only
		//tracker->showCross();
		//cv::waitKey(1);

		//use position mode to have a fast response at the first step, then use PID to 

	}
	return 0;
}
