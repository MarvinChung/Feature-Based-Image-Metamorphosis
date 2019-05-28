#include <opencv2/opencv.hpp>
#include <iostream>

void RGBimage2binary(const cv::Mat& imageMat, cv::Mat& binaryMat, const size_t binaryThreshold)
{
    //Grayscale matrix
    cv::Mat grayscaleMat(imageMat.size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor( imageMat, grayscaleMat, CV_BGR2GRAY );

    //Apply thresholding
    cv::threshold(grayscaleMat, binaryMat, binaryThreshold, 255, cv::THRESH_BINARY);


}


