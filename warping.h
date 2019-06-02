#pragma once
#include <opencv2/opencv.hpp>
#include "LineSegment.h"
#include "LinePair.h"

// unsigned char output_image[3][SIZE][SIZE];
// float t = 0.5; //the parameter
// int x, y; //indexs of destination image's each pixel

float length(int x1, int y1, int x2, int y2) { //the distance between two dots (x1, y1) and (x2, y2)
	return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float find_u(int x, int y, int x1, int y1, int x2, int y2) { //find the parameter u
	int inner_product = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
	float u = inner_product / std::pow(length(x1, y1, x2, y2), 2);

	return u;
}

float find_v(int x, int y, int x1, int y1, int x2, int y2) { //finde the parameter v
	int inner_product = (x - x1) * (y2 - y1) + (y - y1) * (x1 - x2);
	float v = inner_product / length(x1, y1, x2, y2);
	//std::cout << v << std::endl;
	//cv::waitKey(0);
	return v;
}
cv::Vec3b color_interpolation(float x, float y, cv::Vec3b color1, cv::Vec3b color2, cv::Vec3b color3, cv::Vec3b color4) { //get the intensity of a pixel if its index is not an integer
	int x_low, x_high, y_low, y_high;
	cv::Vec3b value;
	x_low = int(std::floor(x));
	x_high = int(std::ceil(x));
	y_low = int(std::floor(y));
	y_high = int(std::ceil(y));
	value = (x - x_low) * (y - y_low) * color1 + (x - x_low) * (y_high - y) * color2 + (x_high - x) * (y - y_low) * color3 + (x_high - x) * (y_high - y) * color4;
	return value;
}


void warp(cv::Mat& src, cv::Mat& des, std::vector<LinePair>& pairs, float p, float a, float b)
{
	
	int x_min = 0;
	int x_max = src.cols;
	int y_min = 0;
	int y_max = src.rows; 

	for(int r = 0; r < src.rows; r++)
	{
		for(int c = 0; c < src.cols; c++)
		{
			float u,v;
		    cv::Point2f DSUM;
			float weightsum = 0;
			float weight;
			float dist;
			for (int i = 0; i < pairs.size(); i++) { //find all the points transformed by the line pairs

				u = find_u(c, r, pairs[i].getDesLine().getStartPoint().x, pairs[i].getDesLine().getStartPoint().y, pairs[i].getDesLine().getEndPoint().x, pairs[i].getDesLine().getEndPoint().y); 
				v = find_v(c, r, pairs[i].getDesLine().getStartPoint().x, pairs[i].getDesLine().getStartPoint().y, pairs[i].getDesLine().getEndPoint().x, pairs[i].getDesLine().getEndPoint().y);
				//std::cout << u << std::endl;
				//std::cout << v << std::endl;

				cv::Point2f perp( pairs[i].getSrcLine().getEndPoint().y - pairs[i].getSrcLine().getStartPoint().y, - pairs[i].getSrcLine().getEndPoint().x + pairs[i].getSrcLine().getStartPoint().x);
				cv::Point2f temp_point = pairs[i].getSrcLine().getStartPoint() + u * (pairs[i].getSrcLine().getEndPoint()-pairs[i].getSrcLine().getStartPoint()) + v * perp / pairs[i].getSrcLine().length();
				
				cv::Point2f displacement = cv::Point2f(c, r) - temp_point;

				if (u < 0)
					dist = length(c, r, pairs[i].getDesLine().getStartPoint().x, pairs[i].getDesLine().getStartPoint().y);
				else if (u > 1)
					dist = length(c, r, pairs[i].getDesLine().getEndPoint().x, pairs[i].getDesLine().getEndPoint().y);
				else
					dist = std::abs(v);
				
				weight = std::pow( std::pow(pairs[i].getDesLine().length(), p) / (a + dist), b);
				weightsum = weightsum + weight;

				DSUM += weight * displacement;
				
			}
			cv::Point2f output = cv::Point2f(c, r) + DSUM / weightsum;
			


			int x_low = int(std::floor(output.x));
			int x_high = int(std::ceil(output.x));
			int y_low = int(std::floor(output.y));
			int y_high = int(std::ceil(output.y));

			x_low = (x_low > x_min && x_low < x_max - 1)? x_low : 0;
			x_high = (x_high > x_min && x_high < x_max - 1)? x_high : 0;

			y_low = (y_low > y_min && y_low < y_max - 1)? y_low : 0;
			y_high = (y_high > y_min && y_high < y_max - 1)? y_high : 0;


			cv::Vec3b color1, color2, color3, color4;

			color1 =  src.at<cv::Vec3b>(cv::Point(x_low ,y_low));
			color2 = src.at<cv::Vec3b>(cv::Point(x_low, y_high));
			color3 = src.at<cv::Vec3b>(cv::Point(x_high, y_low));
			color4 = src.at<cv::Vec3b>(cv::Point(x_high, y_high));

			cv::Vec3b value = color_interpolation(output.x, output.y, color1, color2, color3, color4);

			des.at<cv::Vec3b>(cv::Point(c,r)) = value;

		}

	}


}
