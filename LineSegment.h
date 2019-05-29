#include <opencv2/opencv.hpp>
#include <iostream>
/*
void RGBimage2binary(const cv::Mat& imageMat, cv::Mat& binaryMat, const size_t binaryThreshold)
{
    //Grayscale matrix
    cv::Mat grayscaleMat(imageMat.size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor( imageMat, grayscaleMat, CV_BGR2GRAY );

    //Apply thresholding
    cv::threshold(grayscaleMat, binaryMat, binaryThreshold, 255, cv::THRESH_BINARY);

}
*/
void DrawLineSeg(const cv::Mat& imageMat, const int TL, const int TH, std::vector<cv::Vec4i>& lines)
{
    cv::Mat grayscaleMat(imageMat, cv::COLOR_BGRA2GRAY);
	cv::Mat temp;
	cv::GaussianBlur(imageMat, grayscaleMat, cv::Size(3,3), 0, 0);
	cv::Canny(imageMat, temp, TL, TH, 3);
	cv::HoughLinesP(temp, lines, 1, 2 * CV_PI/180, 100,1000, 50 );
  	}


