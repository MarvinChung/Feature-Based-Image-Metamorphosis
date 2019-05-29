#include <opencv2/opencv.hpp>
#include <iostream>
#include "LineSegment.h"

int main (int argc, char **argv)
{

    //Our color image
	std::cout << "hello:" << argv[1] << std::endl;
    
	
	cv::Mat imageMat = cv::imread(argv[1], cv::CV_LOAD_IMAGE_COLOR );
    if (imageMat.empty())
    {
            std::cerr << "ERROR: Could not read image " << argv[1] << std::endl;
            return 1;
    }
    
	

    //Binary image
    //cv::Mat binaryMat(imageMat.size(), CV_8U);
    
    //RGBimage2binary(imageMat, binaryMat, 100);
	std::vector<cv::Vec4i> lines;
	DrawLineSeg(imageMat, 50, 150, lines);
    for( size_t i = 0; i < lines.size(); i++ )
  	{
    	cv::Vec4i l = lines[i];

   		// oran = float(l[1] / col_size );
    	double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;

    	if(angle  < 5  && angle >=-5   ){
    		//if(1){
        	std::cout << l[0] << "," << l[1] << "," << l[2] << "," << l[3] << std::endl;
        	cv::line( imageMat, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3);
    	}
  	}
 
    //Show the results
    //cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
    //cv::imshow("Output", binaryMat);

    //cv::waitKey(0);
}

// int main (int argc, char **argv)
// {

//     if (argc != 2)
//     {
//             std::cout << "USE: " << argv[0] << " <file_path>" << std::endl;
//             return 1;
//     }

//     //Our color image
//     cv::Mat imageMat = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
//     if (imageMat.empty())
//     {
//             std::cerr << "ERROR: Could not read image " << argv[1] << std::endl;
//             return 1;
//     }

//     //Grayscale matrix
//     cv::Mat grayscaleMat (imageMat.size(), CV_8U);

//     //Convert BGR to Gray
//     cv::cvtColor( imageMat, grayscaleMat, CV_BGR2GRAY );

//     //Binary image
//     cv::Mat binaryMat(grayscaleMat.size(), grayscaleMat.type());

//     //Apply thresholding
//     cv::threshold(grayscaleMat, binaryMat, 100, 255, cv::THRESH_BINARY);

//     //Show the results
//     cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
//     cv::imshow("Output", binaryMat);

//     cv::waitKey(0);

//     return 0;
// }