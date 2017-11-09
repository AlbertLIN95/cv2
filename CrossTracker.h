#include <opencv2\opencv.hpp>

/*
Objective: Find the angle and x,y of the cross pattern in the image by
spliting the input image into 4 ROI (work as four line camera) 
The input image should be or close to bi-modal histogram.
*/

class CrossTracker
{
public:
	CrossTracker();
	~CrossTracker();
	void findCross();   //compute the cross location and angle
	void setROIWidth(int); //Set the width of the fours linecamera
	void showCross(); //show the location of the cross(blue point) and 4 pieces of line (black pints) 
	void updateImg(cv::Mat);//update the input frame 
	int getAngle(); //return the angle of the cross
	int* getLocation();//return the location of the corss [x,y]
	int* getSize();
private:
	static cv::Mat preProcess(cv::Mat); //remove noise and convert to gray scale
	cv::Mat object;   // original frame
	int size[2];  //size of the processed frame
	int width; //width of the 'line camera'
	int angle;   //angle of the cross. NOTE the angle is store as offset length but not degree
	int location[2];//x,y coordinate of cross 
};