#pragma once
#include <opencv2/opencv.hpp>
#include "LineSegment.hpp"
#include "LinePair.hpp"
#include <unistd.h>
// unsigned char output_image[3][SIZE][SIZE];
// float t = 0.5; //the parameter
// float x, y; //indexs of destination image's each pixel

float length(float x1, float y1, float x2, float y2) { //the distance between two dots (x1, y1) and (x2, y2)
	return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float find_u(float x, float y, float x1, float y1, float x2, float y2) { //find the parameter u
	float inner_product = (x - x1) * (x2 - x1) + (y - y1) * (y2 - y1);
	float u = inner_product / std::pow(length(x1, y1, x2, y2), 2);

	return u;
}

float find_v(float x, float y, float x1, float y1, float x2, float y2) { //finde the parameter v
	float inner_product = (x - x1) * (y2 - y1) + (y - y1) * (x1 - x2);
	float v = inner_product / length(x1, y1, x2, y2);
	//std::cout << v << std::endl;
	//cv::waitKey(0);
	return v;
}
cv::Vec3b color_interpolation(float x, float y, cv::Vec3b color1, cv::Vec3b color2, cv::Vec3b color3, cv::Vec3b color4) { //get the floatensity of a pixel if its index is not an floateger
	int x_low, x_high, y_low, y_high;
	cv::Vec3b value;
	x_low = int(std::floor(x));
	x_high = int(std::ceil(x));
	y_low = int(std::floor(y));
	y_high = int(std::ceil(y));

	float a =  x - x_low;
	float b = y - y_low;


	value = (1 - a) * (1 - b) * color1 + a * (1 - b) * color2 + a * b * color3 + (1 - a) * b * color4;
	// printf("x:%lf, x_low:%d, x_high:%d\n", x, x_low, x_high );
	// printf("y:%lf, y_low:%d, y_high:%d\n", y, y_low, y_high );
	// std::cout << color1 << std::endl;
	// std::cout << color2 << std::endl;
	// std::cout << color3 << std::endl;
	// std::cout << color4 << std::endl;
	// printf("value:%lf, %lf ,%lf\n", value[0], value[1], value[2]);
	// sleep(3);

	return value;
}


void warp(const cv::Mat& src, cv::Mat& outputMat, const std::vector<LinePair>& pairs, const float p, const float a, const float b)
{
	
	float x_min = 0;
	float x_max = src.cols;
	float y_min = 0;
	float y_max = src.rows; 

	for(float r = 0; r < src.rows; r++)
	{
		for(float c = 0; c < src.cols; c++)
		{
			float u,v;
		    cv::Point2f DSUM;
			float weightsum = 0;
			float weight;
			float dist;
			for (float i = 0; i < pairs.size(); i++) { //find all the Points transformed by the line pairs

				u = find_u(c, r, pairs[i].getDesLine().getStartPoint().x, pairs[i].getDesLine().getStartPoint().y, pairs[i].getDesLine().getEndPoint().x, pairs[i].getDesLine().getEndPoint().y); 
				v = find_v(c, r, pairs[i].getDesLine().getStartPoint().x, pairs[i].getDesLine().getStartPoint().y, pairs[i].getDesLine().getEndPoint().x, pairs[i].getDesLine().getEndPoint().y);
				//std::cout << u << std::endl;
				//std::cout << v << std::endl;

				cv::Point2f perp( pairs[i].getSrcLine().getEndPoint().y - pairs[i].getSrcLine().getStartPoint().y, - pairs[i].getSrcLine().getEndPoint().x + pairs[i].getSrcLine().getStartPoint().x);
				cv::Point2f temp_Point = pairs[i].getSrcLine().getStartPoint() + u * (pairs[i].getSrcLine().getEndPoint()-pairs[i].getSrcLine().getStartPoint()) + v * perp / pairs[i].getSrcLine().length();
				
				//THIS SHOULD TIMES -1, SINCE WE UPDATE DESTINATION FROM FIND THE SRC POINT.
				cv::Point2f displacement = -(cv::Point2f(c, r) - temp_Point);

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

			x_low = (x_low >= x_min && x_low < x_max )? x_low : 0;
			x_high = (x_high >= x_min && x_high < x_max )? x_high : 0;

			y_low = (y_low >= y_min && y_low < y_max )? y_low : 0;
			y_high = (y_high >+ y_min && y_high < y_max )? y_high : 0;


			cv::Vec3b color1, color2, color3, color4;

			color1 = src.at<cv::Vec3b>(cv::Point(x_low ,y_low));
			color2 = src.at<cv::Vec3b>(cv::Point(x_low, y_high));
			color3 = src.at<cv::Vec3b>(cv::Point(x_high, y_high));
			color4 = src.at<cv::Vec3b>(cv::Point(x_high, y_low));
			

			cv::Vec3b value = color_interpolation(output.x, output.y, color1, color2, color3, color4);

			outputMat.at<cv::Vec3b>(cv::Point(c,r)) = value;

		}

	}


}
