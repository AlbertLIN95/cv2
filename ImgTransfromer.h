#include <opencv2\opencv.hpp>


/*

ImgTransformer can perform both affine and perspective transform
*/
class ImgTransformer
{
public:
	ImgTransformer();
	~ImgTransformer();
	void updateImg(cv::Mat);//update the input frame
	void findInputCorners();//compute the corners of quadrilateral in input image 
	void setOutputCorners(std::vector<cv::Point2f>);//set the output image size
	void updateAffineMatix(); //update affine matix used for affineTransrom.  
	void updatePerspectiveMatix();//update perspective matix used for perspectiveTransrom.
	 //compute the image to real world ration, input is the length of the objective sheet size in mm
	float getRation(float);
	cv::Mat affineTransfrom();//compute affine transfromation and return the output image
	cv::Mat perspectiveTransfrom();
private:
	void preProcess();//remove noise and convert to grayscale
	std::vector<cv::Point2f> corners;//store the fours corners of quadrilateral
	std::vector<cv::Point2f> outputCorners;//obective corners of the output image
	cv::Mat input;//store orginal image
	cv::Mat pInput;//store pre-process image
	cv::Mat output;//store the output image
	cv::Mat affineMatrix;//store the affine transform Matix
	cv::Mat perspectiveMatrix;//store the perspective transform matix
};