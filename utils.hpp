#pragma once
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <limits>

#define RULE_BASE 0
#define MODEL_BASE 1
#define MODEL_EXTRA_LIPS 2

inline float euclideanDist(cv::Point& p, cv::Point& q) {
    cv::Point diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

inline bool bound(int x, int y, cv::Mat image) {
	if(x < 0 || x > image.cols || y < 0 || y > image.rows)
		return false;
	else
		return true;
}

