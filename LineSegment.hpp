#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
// #include <opencv2/face/facemark.hpp>
// #include <opencv2/face/facemarkLBF.hpp>	
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.hpp"
#include "drawLandmarks.hpp"

class LineSegment
{
private:
	cv::Point2f m_start;
	cv::Point2f m_end;
public:
	LineSegment();
	LineSegment(const cv::Vec4i& in);
	LineSegment(const LineSegment& in);
	LineSegment(const cv::Point2f& start, const cv::Point2f& end);

	cv::Point2f mid_Point() const;
	cv::Point2f getStartPoint() const ;
	cv::Point2f getEndPoint() const;
	float midPointdistance(const LineSegment& other) const;
	float length() const;
	friend std::ostream& operator<<(std::ostream& os, const LineSegment& line); 
	bool operator==(const LineSegment& other) const;
	LineSegment operator+(const LineSegment& rhs) const;
	LineSegment operator-(const LineSegment& rhs) const;
	float theta() const;
	float tan() const;
	float dist2Point(cv::Point2f p) const;


};

LineSegment::LineSegment(): m_start(0,0), m_end(0,0)
{}

LineSegment::LineSegment(const cv::Vec4i& in)
{
	m_start = cv::Point2f(in[0], in[1]);
	m_end = cv::Point2f(in[2], in[3]);
}

LineSegment::LineSegment(const cv::Point2f& start, const cv::Point2f& end)
{
	m_start = start;
	m_end = end;
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

LineSegment operator*(const float x, const LineSegment rhs)
{
	return LineSegment(x * rhs.getStartPoint(), x * rhs.getEndPoint());
}

LineSegment operator*(const LineSegment lhs, const float x)
{
	return LineSegment(x * lhs.getStartPoint(), x * lhs.getEndPoint());
}

LineSegment operator/(const LineSegment lhs, const float x)
{
	return LineSegment(lhs.getStartPoint()/x, lhs.getEndPoint()/x);
}

LineSegment LineSegment::operator+(const LineSegment& rhs) const
{
	return LineSegment(m_start + rhs.getStartPoint(), m_end + rhs.getEndPoint());
}

LineSegment LineSegment::operator-(const LineSegment& rhs) const
{
	return LineSegment(m_start - rhs.getStartPoint(), m_end - rhs.getEndPoint());
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
	return atan(this->tan()) * 180 / CV_PI;
}

float LineSegment::length() const
{
	return std::sqrt((m_end.y - m_start.y) * (m_end.y - m_start.y) +  (m_end.x - m_start.x) * (m_end.x - m_start.x));
}

float LineSegment::dist2Point(cv::Point2f p) const
{
	return (tan() * (p.x - m_start.x) - p.y + m_start.y)/ std::sqrt( tan() * tan() + 1);
}

float LineSegment::midPointdistance(const LineSegment& other) const
{
	cv::Point src_mid = this->mid_Point();
	cv::Point other_mid = other.mid_Point();

	return euclideanDist(src_mid, other_mid);
}


//Model Base
inline void getLines(const cv::Mat& imageMat, std::vector<cv::Vec4i>& lines, std::string str)
{
	//Reference
	//https://github.com/spmallick/learnopencv/blob/master/FacialLandmarkDetection/facialLandmarkDetection.cpp

	// [1]Haar Face Detector
    // cv::CascadeClassifier faceDetector("../haarcascade_frontalcatface_extended.xml");
    // cv::CascadeClassifier faceDetector("../haarcascade_frontalface_alt2.xml");
	// [2]LBP Face Detector
	cv::CascadeClassifier faceDetector("../lbpcascade_frontalface_improve.xml");

    // Create an instance of Facemark
    cv::Ptr<cv::face::Facemark> facemark = cv::face::FacemarkLBF::create();

    // Load landmark detector
    facemark->loadModel("../lbfmodel.yaml");

    
	cv::Mat gray;


	// Find face
	std::vector<cv::Rect> faces;
	// Convert frame to grayscale because
    // faceDetector requires grayscale image.
	cv::cvtColor(imageMat, gray, cv::COLOR_BGR2GRAY);


	

	// Detect faces
	faceDetector.detectMultiScale(gray, faces);

	std::cout << "detected faces: " << faces.size() << std::endl;


	// if face detector doesn't work, assume the whole image is face
	if(faces.size() == 0 )
	{
		
		std::cout << "no detected faces! Assume the image is a face." << std::endl;
		faces.clear();
		faces.push_back(cv::Rect(0, 0, imageMat.size().width, imageMat.size().height));
	}
	else if(faces.size() > 1)
	{
		std::cout << "More than one face! Assume the biggest one is a face" << std::endl;
		
		float area = 0;
		cv::Rect biggestRect;

		for(int i = 0 ; i < faces.size(); i++)
		{
			if(faces[i].area() > area)
			{
				area = faces[i].area();
				biggestRect = faces[i];
			}
		}

		faces.clear();
		faces.push_back(biggestRect);
	}


	cv::Mat tp = imageMat.clone();

	rectangle(tp, faces[0], cv::Scalar(255,0,0), 1, 8, 0);
	imwrite(str, tp);
#ifdef DEBUG

	for( int i = 0; i < faces.size(); i++ )
    {
      std::cout << "x" << faces[i].x << std::endl;
      std::cout << "y" << faces[i].y << std::endl;
      cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
      cv::ellipse(gray, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar( 0, 255, 0 ), 2, 8, 0 );
    }
    imshow("[debug] check if image detector works", gray);
    cv::waitKey(0);

#endif
	// Variable for landmarks. 
	// Landmarks for one face is a vector of points
	// There can be more than one face in the image. Hence, we 
	// use a vector of vector of points. 
	std::vector< std::vector<cv::Point2f> > landmarks;

	// Run landmark detector
	bool success = facemark->fit(imageMat, faces, landmarks);

	cv::Mat tempImage = imageMat.clone();

	if(success)
	{
        // If successful, render the landmarks on the face
		for(int i = 0; i < landmarks.size(); i++)
		{
			if( (str == std::string("SrcLiplines.png")) || (str == std::string("DesLiplines.png")) )
				drawLips(tempImage, landmarks[i], lines);
			else
				drawLandmarks(tempImage, landmarks[i], lines);
		}

	}
	else
	{
		std::cerr << "model base failed to fit" << std::endl;
  		exit(1);
	}

	if(lines.size() == 0)
  	{
  		std::cerr << "no line detected" << std::endl;
  		exit(1);
  	}

#ifdef DEBUG 
	for( size_t i = 0; i < lines.size(); i++ )
  	{

        std::cout << lines[i][0] << "," << lines[i][1] << "," << lines[i][2] << "," << lines[i][3] << std::endl;
        cv::line( tempImage, cv::Point(lines[i][0],lines[i][1]), cv::Point(lines[i][2],lines[i][3]), cv::Scalar(i,i,255), 2, 8);
  	}

	cv::namedWindow("[debug] model base getLines image show", cv::WINDOW_AUTOSIZE);
    cv::imshow("[debug] model base getLines image show", tempImage);
    cv::waitKey(0);
#endif
}

//Rule Base
inline void getLines(const cv::Mat& imageMat, const double TL, const double TH, std::vector<cv::Vec4i>& lines, std::string str)
{	
	
	cv::Mat response;
	const double CannyThres[] = {TL, TH};
	const int CannyAperture = 3;
	const bool CannyL2Grad = true;
	cv::Mat gray;
	cv::cvtColor(imageMat, gray, cv::COLOR_BGR2GRAY);
	cv::Canny( gray, response, CannyThres[0], CannyThres[1], CannyAperture, CannyL2Grad );
	
	cv::imwrite(str+"Canny.png", response);

#ifdef DEBUG	
	cv::namedWindow(str, cv::WINDOW_AUTOSIZE);
    cv::imshow(str, response);
    cv::waitKey(0);
#endif

	const double HoughDistRes = 1;
	const double HoughAngRes = CV_PI/180;
	const double HoughMinLen = 20;//30
	const double HoughMaxGap = 5;//10
	const int HoughThres = 5;//80

	lines.clear();
	
	
	
	
	cv::Mat temp_srcMat = imageMat.clone();
	cv::HoughLinesP( response, lines, HoughDistRes, HoughAngRes, HoughThres, HoughMinLen, HoughMaxGap );
 
	for( size_t i = 0; i < lines.size(); i++ )
  	{

        //std::cout << l[0] << "," << l[1] << "," << l[2] << "," << l[3] << std::endl;
        cv::line( temp_srcMat, cv::Point(lines[i][0],lines[i][1]), cv::Point(lines[i][2],lines[i][3]), cv::Scalar(i,i,255), 2, 8);
  	}
    
  	if(lines.size() == 0)
  	{
  		std::cerr << "no line detected" << std::endl;
  		exit(1);
  	}

#ifdef DEBUG 
	cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
    cv::imshow(str, temp_srcMat);
    cv::waitKey(0);
#endif
}

//void DrawLineSeg(const cv::Mat& imageMat, const double TL, const double TH, std::vector<cv::Vec4i>& lines);


