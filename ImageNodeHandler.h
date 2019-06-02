#pragma once
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

#include "warping.h"
#include "utils.h"

class ImageNodeHandler
{
public:
	int m_imageCt;
	int m_nodeCt;
	int m_maxDepth;
	float m_p, m_a, m_b, m_deltaT;

	ImageNodeHandler();
	ImageNodeHandler(int imageCt, int nodeCt, float p, float a, float b, float deltaT, int maxDepth);
	void drawImage(cv::Mat& mat);
	void buildRecursive(int depth, cv::Mat& srcMat, cv::Mat& desMat);
};

ImageNodeHandler::ImageNodeHandler() : m_imageCt(0), m_nodeCt(0), m_p(0.5), m_a(0.2), m_b(1), m_deltaT(0.2), m_maxDepth(3)
{}

ImageNodeHandler::ImageNodeHandler(int imageCt, int nodeCt, float p, float a, float b, float deltaT, int maxDepth) : 
m_imageCt(imageCt), m_nodeCt(nodeCt), m_p(p), m_a(a), m_b(b), m_deltaT(deltaT), m_maxDepth(maxDepth)
{}

void ImageNodeHandler::drawImage(cv::Mat& mat)
{
	std::string filename = std::to_string(m_imageCt) + ".png";
	cv::imwrite(filename, mat);
	m_imageCt++;
	std::cout << "image: " << filename << " written!" << std::endl;
}

void ImageNodeHandler::buildRecursive(int depth, cv::Mat& srcMat, cv::Mat& desMat)
{
	if(depth == m_maxDepth)
		return;

	m_nodeCt++;

	std::vector<cv::Vec4i> linesA;
	getLines(srcMat, 50, 150, linesA, "src");
	
	std::vector<cv::Vec4i> linesB;
	getLines(desMat, 50, 150, linesB, "des");

	std::vector<LinePair> pairs;
	generateLinePairs(linesA, linesB, pairs);
	drawLinePairs(srcMat, desMat, pairs);

	cv::Mat postFirstMat = desMat.clone();

	warp(srcMat, postFirstMat, pairs, m_p, m_a, m_b);

	buildRecursive(depth + 1, srcMat, postFirstMat);

	//drawImage(postFirstMat);

	
	cv::Mat preLastMat = desMat.clone();
	warp(desMat, preLastMat, pairs, m_p, m_a, m_b);
	
	
	cv::Mat outputMat = cv::Mat::zeros(srcMat.rows, srcMat.cols, CV_8UC3 );
	for(float t = 0; t < 1; t += m_deltaT)
	{
		for(int r = 0; r < srcMat.rows; r++)
		{
			for(int c = 0; c< srcMat.cols; c++)
			{
				cv::Vec3b color1, color2;
				if(bound(c, r, postFirstMat))
					color1 = postFirstMat.at<cv::Vec3b>(cv::Point(c ,r));
				if(bound(c, r, preLastMat))
					color2 = preLastMat.at<cv::Vec3b>(cv::Point(c, r));
				//std::cout<< r << "," << c <<std::endl;
				//std::cout<< color1<<std::endl;
				//std::cout << color2 <<std::endl;
				outputMat.at<cv::Vec3b>(cv::Point(c, r)) = (1-t) * color1 + t * color2 ;
			}
		}

		drawImage(outputMat);
	}
	
	buildRecursive(depth + 1, preLastMat, desMat);
}