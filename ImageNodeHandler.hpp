#pragma once
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

#include "warping.hpp"
#include "utils.hpp"

class ImageNodeHandler
{
public:
	int m_frameCt;
	int m_maxFrame;
	int m_maxDepth;
	float m_p, m_a, m_b;

	ImageNodeHandler();
	ImageNodeHandler(int imageCt, int nodeCt, float p, float a, float b);
	void drawImage(const cv::Mat& mat);
	void GenerateAnimation(const cv::Mat& srcMat, const cv::Mat& desMat, int mode);
};

ImageNodeHandler::ImageNodeHandler() : m_frameCt(0), m_maxFrame(10), m_p(0.5), m_a(0.2), m_b(1)
{}

ImageNodeHandler::ImageNodeHandler(int frameCt, int maxFrame, float p, float a, float b) : 
m_frameCt(frameCt), m_maxFrame(maxFrame), m_p(p), m_a(a), m_b(b)
{}

void ImageNodeHandler::drawImage(const cv::Mat& mat)
{
	std::string filename = std::to_string(m_frameCt) + ".png";
	cv::imwrite(filename, mat);
	m_frameCt++;
	std::cout << "image: " << filename << " written!" << std::endl;
}

void ImageNodeHandler::GenerateAnimation(const cv::Mat& srcMat, const cv::Mat& desMat, int mode) 
{

	std::vector<cv::Vec4i> linesA;
	std::vector<cv::Vec4i> linesB;

	if(mode == RULE_BASE)
	{
		getLines(srcMat, 50, 150, linesA, "src");
		getLines(desMat, 50, 150, linesB, "des");
	}
	else if(mode == MODEL_BASE){
		getLines(srcMat, linesA, "Srclines.png");
		getLines(desMat, linesB, "Deslines.png");
	}	
	else if(mode == MODEL_EXTRA_LIPS)
	{
		getLines(srcMat, linesA, "SrcLiplines.png");
		getLines(desMat, linesB, "DesLiplines.png");
	}
	

	

	std::vector<LinePair> pairs;
	generateLinePairs(linesA, linesB, pairs);
	drawLinePairs(srcMat, desMat, pairs, "originalpairs");



	std::cout << m_frameCt << std::endl;
	std::cout << m_maxFrame << std::endl;

	while( m_frameCt <= m_maxFrame )
	{
		float ratio = float(m_frameCt)/m_maxFrame;

		std::vector<LinePair> left_pairs;
		std::vector<LinePair> right_pairs;	

		for(int i = 0; i < pairs.size(); i++)
		{
			LineSegment warpLine = (1.0 - ratio) * pairs[i].getSrcLine() + ratio * pairs[i].getDesLine();
			left_pairs.push_back(LinePair(pairs[i].getSrcLine(), warpLine));
			right_pairs.push_back(LinePair(pairs[i].getDesLine(), warpLine));
		}

// #ifdef DEBUG		
// 		drawLinePairs(srcMat, desMat, left_pairs);
// 		drawLinePairs(desMat, srcMat, right_pairs);
// #endif
		cv::Mat postMat = cv::Mat::zeros(srcMat.rows, srcMat.cols, CV_8UC3 );
		cv::Mat preMat = cv::Mat::zeros(desMat.rows, desMat.cols, CV_8UC3 );


		warp(srcMat, postMat, left_pairs, m_p, m_a, m_b);
		warp(desMat, preMat, right_pairs, m_p, m_a, m_b);

		std::string filename1 = "postMat" + std::to_string(m_frameCt) + ".png";
		cv::imwrite(filename1, postMat);
		std::string filename2 = "preMat" + std::to_string(m_frameCt) + ".png";
		cv::imwrite(filename2, preMat);

#ifdef DEBUG		
		cv::imshow("[debug] postMat",postMat);
		cv::imshow("[debug] preMat", preMat);
#endif
		cv::Mat outputMat = cv::Mat::zeros(srcMat.rows, srcMat.cols, CV_8UC3 );

		

		for(int r = 0; r < srcMat.rows; r++)
		{
			for(int c = 0; c< srcMat.cols; c++)
			{
				cv::Vec3b color1, color2;
				if(bound(c, r, postMat))
					color1 = postMat.at<cv::Vec3b>(cv::Point(c, r));
				if(bound(c, r, preMat))
					color2 = preMat.at<cv::Vec3b>(cv::Point(c, r));

				outputMat.at<cv::Vec3b>(cv::Point(c, r)) = (1.0-ratio) * color1 + ratio * color2 ;

			}
		}

		drawImage(outputMat);
		
	
	}

}