#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

// Reference:
//    [1]Tutorial: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/
//    [2]Code: https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection


// drawPolyLine draws a poly line by joining 
// successive points between the start and end indices. 
inline void drawPolyline
( 
    cv::Mat &im,
    const std::vector<cv::Point2f> &landmarks,
    const int start,
    const int end,
    bool isClosed = false
)
{
    // Gather all points between the start and end indices
    std::vector <cv::Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }
#ifdef DEBUG
    // Draw polylines. 
    cv::polylines(im, points, isClosed, cv::Scalar(255, 200,0), 2, 16);       
#endif
}

inline void drawStraightline
( 
    cv::Mat &im,
    const std::vector<cv::Point2f> &landmarks,
    std::vector<cv::Vec4i>& lines,
    const int start,
    const int end,    
    bool isClosed = false
)
{
    // Gather all points between the start and end indices
    std::vector <cv::Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }

    for(int i = 1; i < points.size(); i++)
    {
        lines.push_back(cv::Vec4i(points[i-1].x, points[i-1].y, points[i].x, points[i].y));
    }
}


inline void drawLips(cv::Mat &im, std::vector<cv::Point2f>& landmarks, std::vector<cv::Vec4i>& lines)
{
    // Draw face for the 68-point model.
    std::cout << landmarks.size() << " landmarks have found" << std::endl;

    if (landmarks.size() == 68)
    {
        /*
        drawPolyline(im, landmarks, 0, 16);           // Jaw line
        drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
        drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
        drawPolyline(im, landmarks, 27, 30);          // Nose bridge
        drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
        drawPolyline(im, landmarks, 36, 41, true);    // Left eye
        drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
        drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
        drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
        */

        /*
        drawStraightline(im, landmarks, lines, 0, 16);           // Jaw line
        drawStraightline(im, landmarks, lines, 17, 21);          // Left eyebrow
        drawStraightline(im, landmarks, lines, 22, 26);          // Right eyebrow
        drawStraightline(im, landmarks, lines, 27, 30);          // Nose bridge
        drawStraightline(im, landmarks, lines, 30, 35, true);    // Lower nose
        drawStraightline(im, landmarks, lines, 36, 41, true);    // Left eye
        drawStraightline(im, landmarks, lines, 42, 47, true);    // Right Eye
        drawStraightline(im, landmarks, lines, 48, 59, true);    // Outer lip
        drawStraightline(im, landmarks, lines, 60, 67, true);    // Inner lip
        */
        drawStraightline(im, landmarks, lines, 48, 59, true);    // Outer lip
        drawStraightline(im, landmarks, lines, 60, 67, true);    // Inner lip
    }
    else 
    { 
        // If the number of points is not 68, we do not know which 
        // points correspond to which facial features. So, we draw 
        // one dot per landamrk. 
        std::vector <cv::Point> points;
        int size = std::min(19, int(landmarks.size()));
        for(int i = 0; i < size; i++)
        {
            cv::circle(im,landmarks[i],3, cv::Scalar(255, 200,0), cv::FILLED);
            points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
        }

        for(int i = 1; i < points.size(); i++)
        {
            lines.push_back(cv::Vec4i(points[i-1].x, points[i-1].y, points[i].x, points[i].y));
        }
    }
        
}

inline void drawLandmarks(cv::Mat &im, std::vector<cv::Point2f>& landmarks, std::vector<cv::Vec4i>& lines)
{
    // Draw face for the 68-point model.
    std::cout << landmarks.size() << " landmarks have found" << std::endl;

    if (landmarks.size() == 68)
    {
        /*
        drawPolyline(im, landmarks, 0, 16);           // Jaw line
        drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
        drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
        drawPolyline(im, landmarks, 27, 30);          // Nose bridge
        drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
        drawPolyline(im, landmarks, 36, 41, true);    // Left eye
        drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
        drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
        drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
        */

        drawStraightline(im, landmarks, lines, 0, 16);           // Jaw line
        /*
        drawStraightline(im, landmarks, lines, 17, 21);          // Left eyebrow
        drawStraightline(im, landmarks, lines, 22, 26);          // Right eyebrow
        drawStraightline(im, landmarks, lines, 27, 30);          // Nose bridge
        drawStraightline(im, landmarks, lines, 30, 35, true);    // Lower nose
        drawStraightline(im, landmarks, lines, 36, 41, true);    // Left eye
        drawStraightline(im, landmarks, lines, 42, 47, true);    // Right Eye
        drawStraightline(im, landmarks, lines, 48, 59, true);    // Outer lip
        drawStraightline(im, landmarks, lines, 60, 67, true);    // Inner lip
        */
    }
    else 
    { 
        // If the number of points is not 68, we do not know which 
        // points correspond to which facial features. So, we draw 
        // one dot per landamrk. 
        std::vector <cv::Point> points;
        int size = std::min(16, int(landmarks.size()));
        for(int i = 0; i < size; i++)
        {
            cv::circle(im,landmarks[i],3, cv::Scalar(255, 200,0), cv::FILLED);
            points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
        }

        for(int i = 1; i < points.size(); i++)
        {
            lines.push_back(cv::Vec4i(points[i-1].x, points[i-1].y, points[i].x, points[i].y));
        }
    }
        
}