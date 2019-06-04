#include <opencv2/opencv.hpp>

#include <iostream>

#include "warping.hpp"
#include "utils.hpp"
#include "ImageNodeHandler.hpp"

int main (int argc, char **argv)
{

    //Our color image
	std::cout << "filename:" << argv[1] << std::endl;
	std::cout << "filename:" << argv[2] << std::endl;
    
	
	cv::Mat srcMat = cv::imread(argv[1], 1);
    cv::Mat desMat = cv::imread(argv[2], 1);
	
	if (srcMat.empty() || desMat.empty())
    {
        std::cerr << "ERROR: Could not read image " << argv[1] << std::endl;
        return 1;
    }
    
	std::cout << "resize images" << std::endl;
	
	//rule base
	//cv::resize(srcMat, srcMat, cv::Size(256, 256), 0, 0, cv::INTER_LANCZOS4);	
	//cv::resize(desMat, desMat, cv::Size(256, 256), 0, 0, cv::INTER_LANCZOS4);

	//model base
	//cv::resize(srcMat, srcMat, cv::Size(256, 150), 0, 0, cv::INTER_LANCZOS4);	
	//cv::resize(desMat, desMat, cv::Size(256, 150), 0, 0, cv::INTER_LANCZOS4);		

	cv::resize(srcMat, srcMat, cv::Size(256, 256), 0, 0, cv::INTER_LANCZOS4);	
	cv::resize(desMat, desMat, cv::Size(256, 256), 0, 0, cv::INTER_LANCZOS4);		

	//void warp(cv::Mat& src, cv::Mat& des, std::vector<LinePair>& pairs, float p, float a, float b)
	
	//parameters for warping
	float p = 0; // suggest 0 ~ 1
	float a = 0.2; //Values larger than that will yield a more smooth warping,but with less precise control.
	float b = 1; // suggest 0.5 ~ 2

	//image_ct set to 1, since the 0.png is srcMat
	ImageNodeHandler root(0, 20, p, a, b);
	//ImageNodeHandler root;

	root.drawImage(srcMat);

	root.GenerateAnimation(srcMat, desMat, 1);

	root.drawImage(desMat);

}

