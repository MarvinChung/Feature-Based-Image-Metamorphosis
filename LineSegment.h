#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.h"

class LineSegment
{
private:
	cv::Point2f m_start;
	cv::Point2f m_end;
public:
	LineSegment();
	LineSegment(const cv::Vec4i& in);
	LineSegment(const LineSegment& in);
	cv::Point2f mid_Point() const;
	cv::Point2f getStartPoint() const ;
	cv::Point2f getEndPoint() const;
	float midPointdistance(const LineSegment& other) const;
	float length() const;
	friend std::ostream& operator<<(std::ostream& os, const LineSegment& line);
	bool operator==(const LineSegment& other) const;
	float theta() const;
	float tan() const;
};

LineSegment::LineSegment(): m_start(0,0), m_end(0,0)
{}

LineSegment::LineSegment(const cv::Vec4i& in)
{
	m_start = cv::Point2f(in[0], in[1]);
	m_end = cv::Point2f(in[2], in[3]);
}

LineSegment::LineSegment(const LineSegment& in)
{
	m_start = in.m_start;
	m_end = in.m_end;
}

std::ostream& operator<<(std::ostream& os, const LineSegment& line)
{
	os << '(' << line.m_start.x << ',' << line.m_start.y << ')' << '(' << line.m_end.x << ',' << line.m_end.y << ')' ;
	return os;
}

bool LineSegment::operator==(const LineSegment& other) const
{
    return (m_start.x == other.m_start.x) && (m_start.y == other.m_start.y) && (m_end.x == other.m_end.x) && (m_end.y == other.m_end.y);
}

cv::Point2f LineSegment::mid_Point() const
{
	return (m_start + m_end) / 2;
}

cv::Point2f LineSegment::getStartPoint() const
{
	return m_start;
}

cv::Point2f LineSegment::getEndPoint() const
{
	return m_end;
}

float LineSegment::tan() const
{
	return (m_end.y - m_start.y)/(m_end.x - m_start.x);
}

float LineSegment::theta() const
{
	return atan(this->tan());
}

float LineSegment::length() const
{
	return std::sqrt((m_end.y - m_start.y) * (m_end.y - m_start.y) +  (m_end.x - m_start.x) * (m_end.x - m_start.x));
}

float LineSegment::midPointdistance(const LineSegment& other) const
{
	cv::Point src_mid = this->mid_Point();
	cv::Point other_mid = other.mid_Point();

	return euclideanDist(src_mid, other_mid);
}

inline void getLines(const cv::Mat& imageMat, const double TL, const double TH, std::vector<cv::Vec4i>& lines, std::string str)
{
	//cv::Mat grayscaleMat (imageMat.size(), CV_8U);
	//cv::Mat temp;
	//cv::GaussianBlur(imageMat, grayscaleMat, cv::Size(3,3), 0, 0);
	//cv::Canny(imageMat, temp, TL, TH, 3);
	//cv::HoughLinesP(temp, lines, 1, 2 * CV_PI/180, 100,1000, 50 );
	std::cout << "Canny" << std::endl;
	
	cv::Mat response;
	const double CannyThres[] = {TL, TH};
	const int CannyAperture = 3;
	const bool CannyL2Grad = true;
	cv::Mat gray;
	cv::cvtColor(imageMat, gray, cv::COLOR_BGR2GRAY);
	cv::Canny( gray, response, CannyThres[0], CannyThres[1], CannyAperture, CannyL2Grad );
	
//#ifdef DEBUG	
	cv::namedWindow(str, cv::WINDOW_AUTOSIZE);
    cv::imshow(str, response);
    cv::waitKey(0);
//#endif

	const double HoughDistRes = 2;
	const double HoughAngRes = CV_PI/180;
	const double HoughMinLen = 20;//30
	const double HoughMaxGap = 5;//10
	const int HoughThres = 10;//80

	lines.clear();
	
	std::cout << "HoughLinesP" << std::endl;
	
	cv::HoughLinesP( response, lines, HoughDistRes, HoughAngRes, HoughThres, HoughMinLen, HoughMaxGap );
 

}

//void DrawLineSeg(const cv::Mat& imageMat, const double TL, const double TH, std::vector<cv::Vec4i>& lines);


