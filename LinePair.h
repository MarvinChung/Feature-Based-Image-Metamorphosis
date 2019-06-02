#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

#include "LineSegment.h"
#include "utils.h"

class LinePair
{
private:
	LineSegment m_src_line;
	LineSegment m_des_line;
public:
	LinePair();
	LinePair(const LineSegment& src, const LineSegment& des);
	LineSegment getSrcLine() const;
	LineSegment getDesLine() const;

};


LinePair::LinePair(): m_src_line(), m_des_line()
{}

LinePair::LinePair(const LineSegment& src, const LineSegment& des)
{
	m_src_line = src;
	m_des_line = des;
}

LineSegment LinePair::getSrcLine() const
{
	return m_src_line;
}

LineSegment LinePair::getDesLine() const
{
	return m_des_line;
}

inline void drawLinePairs(const cv::Mat& srcMat, const cv::Mat& desMat, const std::vector<LinePair>& pairs)
{
	cv::Mat temp_srcMat = srcMat.clone();
	cv::Mat temp_desMat = desMat.clone();

	for( size_t i = 0; i < pairs.size(); i++ )
  	{

        //std::cout << l[0] << "," << l[1] << "," << l[2] << "," << l[3] << std::endl;
        cv::line( temp_desMat, pairs[i].getDesLine().getStartPoint(), pairs[i].getDesLine().getEndPoint(), cv::Scalar(i,i,255), 2, 8);
    	cv::circle(temp_desMat, pairs[i].getDesLine().getStartPoint(), 5, cv::Scalar(100, 100, 100), 1, 8, 0);
    	cv::circle(temp_desMat, pairs[i].getDesLine().getEndPoint(), 5, cv::Scalar(100, 100, 100), 1, 8, 0);

    	cv::line( temp_srcMat, pairs[i].getSrcLine().getStartPoint(), pairs[i].getSrcLine().getEndPoint(), cv::Scalar(i,i,255), 2, 8);
    	cv::circle(temp_srcMat, pairs[i].getSrcLine().getStartPoint(), 5, cv::Scalar(100, 100, 100), 1, 8, 0);
    	cv::circle(temp_srcMat, pairs[i].getSrcLine().getEndPoint(), 5, cv::Scalar(100, 100, 100), 1, 8, 0);
  	}
     
#ifdef DEBUG
     cv::imshow("srcMat", temp_srcMat);
     cv::imshow("desMat", temp_desMat);
     cv::waitKey(0);
#endif
}

inline void generateLinePairs(const std::vector<cv::Vec4i>&  linesA, const std::vector<cv::Vec4i>&  linesB, std::vector<LinePair>& out_LinePairs)
{
	//std::vector<LinePair> temp_LinePairs;
	out_LinePairs.clear();
	for(int i = 0; i < linesA.size(); i++)
	{
		LineSegment src_line(linesA[i]);
		
		LineSegment candidate_des_line;
		float min_distance = std::numeric_limits<float>::max();
		float min_theta = 360;
		for(int j = 0 ; j< linesB.size(); j++)
		{
			LineSegment des_line(linesB[j]);
			float distance = src_line.midPointdistance(des_line);
			float thetaA = src_line.theta();
			float thetaB = des_line.theta();
			float deltaTheta = std::abs(thetaA - thetaB);
			if(distance < min_distance && deltaTheta < min_theta)
			{
				min_distance = distance;
				min_theta = deltaTheta;
				candidate_des_line = des_line;
			}
		}

		//temp_LinePairs.push_back(LinePair(src_line, candidate_des_line));
		//std::cout << src_line <<std::endl;
		//std::cout << candidate_des_line << std::endl;

		out_LinePairs.push_back(LinePair(src_line, candidate_des_line));
	}


	// for(int i = 0; i < temp_LinePairs.size(); i++)
	// {
	// 	bool push_permission = true;
	// 	//std::cout << "i: " <<  i << std::endl;
	// 	//std::cout << "size: " << temp_LinePairs.size() << std::endl;
	// 	for(int j = 0; j < temp_LinePairs.size();j++)
	// 	{
	// 		if(i == j)
	// 			continue;

	// 		if(temp_LinePairs[i].getDesLine() == temp_LinePairs[j].getDesLine())
	// 		{
				
	// 			if(temp_LinePairs[i].getSrcLine().midPointdistance(temp_LinePairs[i].getDesLine()) > temp_LinePairs[j].getSrcLine().midPointdistance(temp_LinePairs[j].getDesLine()))
	// 			{
	// 				push_permission = false;
	// 				//std::cout << "false" <<std::endl;
	// 				break;
	// 			}
	// 		}
	// 	}

	// 	for(int j = 0 ; j< out_LinePairs.size(); j++)
	// 	{
	// 		if((temp_LinePairs[i].getDesLine() == out_LinePairs[j].getDesLine()) || (temp_LinePairs[i].getSrcLine() == out_LinePairs[j].getSrcLine()))
	// 		{
	// 			push_permission = false;
	// 			break;
	// 		}
	// 	}

	// 	if(push_permission)
	// 	{
	// 		std::cout << i << std::endl;
	// 		std::cout << temp_LinePairs[i].getSrcLine() << std::endl;
	// 		std::cout << temp_LinePairs[i].getDesLine() << std::endl;
	// 		out_LinePairs.push_back(temp_LinePairs[i]);
	// 	}
	// } 

}
