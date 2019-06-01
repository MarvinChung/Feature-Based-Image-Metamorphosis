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


void warp(cv::Mat& src, cv::Mat& des, std::vector<LinePair>& pairs, float t, float a, float b)
{
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
				
				weight = std::pow(pairs[i].getDesLine().length() / (a + dist), b);
				weightsum = weightsum + weight;

				DSUM += weight * displacement;
				
			}
			cv::Point2f output = cv::Point2f(c, r) + DSUM / weightsum;
			
			int x_low = int(std::floor(output.x));
			int x_high = int(std::ceil(output.x));
			int y_low = int(std::floor(output.y));
			int y_high = int(std::ceil(output.y));
			/*
			for (int k = 0; k < 3; k++) {
				float value = color_interpolation(output.x, output.y, image1[k][x_low][y_low], image_data[k][x_low][y_high], image_data[k][x_high][y_low], image_data[k][x_high][y_high]);
				output_image[k][x][y] = unsigned char(round((1 - t) * value + t * image2[k][x][y])); //cross dissolve the color between the new pixel of the source image(output_x, output_y) and the original pixel of the destination page(x, y)
			}
			*/

			// std::cout << x_low << "?" << y_low << std::endl;
			// std::cout << des.rows << "x" << des.cols << std::endl;

			cv::Vec3b color1, color2, color3, color4;
			if(bound(x_low, y_low, src))
				color1 =  src.at<cv::Vec3b>(cv::Point(x_low ,y_low));
			if(bound(x_low, y_high, src))
				color2 = src.at<cv::Vec3b>(cv::Point(x_low, y_high));
			if(bound(x_high, y_low, src))
				color3 = src.at<cv::Vec3b>(cv::Point(x_high, y_low));
			if(bound(x_high, y_high, src))
				color4 = src.at<cv::Vec3b>(cv::Point(x_high, y_high));

			//puts("interpolation");
			cv::Vec3b value = color_interpolation(output.x, output.y, color1, color2, color3, color4);

			//std::cout << value << std::endl;

			des.at<cv::Vec3b>(cv::Point(c,r)) = std::round(1-t) * value + t * src.at<cv::Vec3b>(cv::Point(c,r));

		}

	}


}
