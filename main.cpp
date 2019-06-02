#include <opencv2/opencv.hpp>

#include <iostream>

#include "warping.h"
#include "utils.h"
#include "ImageNodeHandler.h"

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

	//void warp(cv::Mat& src, cv::Mat& des, std::vector<LinePair>& pairs, float p, float a, float b)
	
	//parameters for warping
	float p = 0; // suggest 0 ~ 1
	float a = 0.2; //Values larger than that will yield a more smooth warping,but with less precise control.
	float b = 1; // suggest 0.5 ~ 2
	float deltaT = 0.2;
	int maxDepth = 3;

	//image_ct set to 1, since the 0.png is srcMat
	ImageNodeHandler root(0, 0, p, a, b, deltaT, maxDepth);
	//ImageNodeHandler root;
	std::cout << "m_deltaT: " << root.m_deltaT << std::endl;

	if(root.m_deltaT <= 0)
	{
		std::cerr << "ERROR: m_deltaT <=0 will cause program never terminate" << std::endl;
		return 1;
	}

	root.drawImage(srcMat);

	root.buildRecursive(0, srcMat, desMat);

	root.drawImage(desMat);

}

