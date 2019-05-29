#include <opencv2/opencv.hpp>
#include <iostream>
/*
void RGBimage2binary(const cv::Mat& imageMat, cv::Mat& binaryMat, const size_t binaryThreshold)
{
    //Grayscale matrix
    cv::Mat grayscaleMat(imageMat.size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor( imageMat, grayscaleMat, CV_BGR2GRAY );

    //Apply thresholding
    cv::threshold(grayscaleMat, binaryMat, binaryThreshold, 255, cv::THRESH_BINARY);

}
*/
void DrawLineSeg(const cv::Mat& imageMat, const double TL, const double TH, std::vector<cv::Vec4i>& lines)
{
	//cv::Mat grayscaleMat (imageMat.size(), CV_8U);
	//cv::Mat temp;
	//cv::GaussianBlur(imageMat, grayscaleMat, cv::Size(3,3), 0, 0);
	//cv::Canny(imageMat, temp, TL, TH, 3);
	//cv::HoughLinesP(temp, lines, 1, 2 * CV_PI/180, 100,1000, 50 );
	cv::Mat response;
	const double CannyThres[] = {50, 200};
	const int CannyAperture = 3;
	const bool CannyL2Grad = true;
	cv::Mat gray;
	cv::cvtColor(imageMat, gray, cv::COLOR_BGR2GRAY);
	cv::Canny( gray, response, CannyThres[0], CannyThres[1], CannyAperture, CannyL2Grad );
	
	cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
    cv::imshow("Output", response);
    cv::waitKey(0);

	const double HoughDistRes = 1;
	const double HoughAngRes = CV_PI/360;
	const double HoughMinLen = 30;//30
	const double HoughMaxGap = 50;//10
	const int HoughThres = 30;//80

	lines.clear();

	cv::HoughLinesP( response, lines, HoughDistRes, HoughAngRes, HoughThres, HoughMinLen, HoughMaxGap );
	for( size_t i = 0; i < lines.size(); i++ )
  	{
    	cv::Vec4i l = lines[i];

   		// oran = float(l[1] / col_size );
    	double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;

    	//if(angle  < 5  && angle >=-5   ){
    		//if(1){
        	std::cout << l[0] << "," << l[1] << "," << l[2] << "," << l[3] << std::endl;
        	cv::line( imageMat, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 2, 8);
    	//}
  	}
     
	 cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
     cv::imshow("Output", imageMat);
     cv::waitKey(0);
 

}

