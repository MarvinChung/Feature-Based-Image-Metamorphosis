#include <opencv2/opencv.hpp>
#include <iostream>

void RGBimage2binary(const cv::Mat& imageMat, cv::Mat& binaryMat, const size_t binaryThreshold)
{
    //Grayscale matrix
    cv::Mat grayscaleMat(imageMat.size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor( imageMat, grayscaleMat, CV_BGR2GRAY );

    //Apply thresholding
    cv::threshold(grayscaleMat, binaryMat, binaryThreshold, 255, cv::THRESH_BINARY);

}

void DrawLineSeg(const cv::Mat& imageMat, const int TL, const int TH, vector<cv::Vec4i>& lines)
{
    cv::Mat grayscaleMat(imageMat, cv2.COLOR_BGR2GRAY);
	cv::Mat temp;
	cv::GaussianBlur(imageMat, grayscaleMat, Size(3,3), 0, 0);
	cv::Canny(imageMat, temp, TL, TH, 3);
	cv::HoughLinesP(temp, lines, 1, 2 * cv::CV_PI/180, 100,1000, 50 );
  	for( size_t i = 0; i < lines.size(); i++ )
  	{
    	cv::Vec4i l = lines[i];

   		// oran = float(l[1] / col_size );
    	double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;

    	if(angle  < 5  && angle >=-5   ){
    		//if(1){
        	cout << l[0] << "," << l[1] << "," << l[2] << "," << l[3] << endl;
        	cv::line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
    	}
  	}
}


