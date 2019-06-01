#include <opencv2/opencv.hpp>

#include <iostream>

#include "warping.h"
#include "utils.h"

int main (int argc, char **argv)
{

    //Our color image
	std::cout << "filename:" << argv[1] << std::endl;
	std::cout << "filename:" << argv[2] << std::endl;
    
	
	cv::Mat srcMat = cv::imread(argv[1]);
    cv::Mat desMat = cv::imread(argv[2]);
	
	if (srcMat.empty() || desMat.empty())
    {
            std::cerr << "ERROR: Could not read image " << argv[1] << std::endl;
            return 1;
    }
    
	std::cout << "resize images" << std::endl;
	cv::resize(srcMat, srcMat, cv::Size(512, 512), 0, 0, CV_INTER_LANCZOS4);	
	cv::resize(desMat, desMat, cv::Size(512, 512), 0, 0, CV_INTER_LANCZOS4);	

	std::vector<cv::Vec4i> linesA;
	getLines(srcMat, 100, 200, linesA, "src");
	
	std::vector<cv::Vec4i> linesB;
	getLines(desMat, 50, 100, linesB, "des");

	std::vector<LinePair> pairs;
	generateLinePairs(linesA, linesB, pairs);
	drawLinePairs(srcMat, desMat, pairs);

	//void warp(cv::Mat& src, cv::Mat& des, std::vector<LinePair>& pairs, float t, float a, float b)
	warp(srcMat, desMat, pairs, 0.5, 1, 2);
 	
	cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
    cv::imshow("Output", desMat);

    cv::waitKey(0);

	/*   
	//Show the results
    for(int i = 19; i < 832 ; i++)
	{
		srcMat.at<cv::Vec3b>(y,x)[0] = 0;
		srcMat.at<cv::Vec3b>(y,x)[1] = 0;
		srcMat.at<cv::Vec3b>(y,x)[2] = 0;
	}
	cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
    cv::imshow("Output", srcMat);

    cv::waitKey(0);
	*/

}

