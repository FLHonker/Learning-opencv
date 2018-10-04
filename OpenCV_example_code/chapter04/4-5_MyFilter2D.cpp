// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-5_MyFilter2D.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 图像掩码操作
# @Create Time: 2018-10-02 19:53:04
# @Last Modified: 2018-10-02 19:53:04
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp> 
#include <iostream> 
#include <stdio.h>
using namespace cv; 
using namespace std;
// 基于像素邻域掩码操作
cv::Mat Myfilter2D(cv::Mat srcImage)
{	
	const int nChannels = srcImage.channels();
    cv::Mat resultImage(srcImage.size(),srcImage.type());
    for(int j = 1 ; j < srcImage.rows-1; ++j)
    {
    	// 获取邻域指针
        const uchar* previous = srcImage.ptr<uchar>(j - 1);
        const uchar* current  = srcImage.ptr<uchar>(j );
        const uchar* next     = srcImage.ptr<uchar>(j + 1);
        uchar* output = resultImage.ptr<uchar>(j);
        for(int i= nChannels;i < nChannels*(srcImage.cols-1); ++i)
        {
        	// 4-邻域均值掩码操作 
            *output++ = saturate_cast<uchar>(
            	(current[i-nChannels] + current[i+nChannels] +
            	 previous[i] + next[i])/4);
        }
    }
    // 边界处理
    resultImage.row(0).setTo(Scalar(0));
    resultImage.row(resultImage.rows-1).setTo(Scalar(0));
    resultImage.col(0).setTo(Scalar(0));
    resultImage.col(resultImage.cols-1).setTo(Scalar(0));
	return resultImage;
}
// 自带库掩码操作
cv::Mat filter2D_(cv::Mat srcImage)
{
    cv::Mat resultImage(srcImage.size(),srcImage.type());
    Mat kern = (Mat_<float>(3,3) <<  0,  1,  0,
                                    1,  0,  1,
                                    0,  1,  0)/ (float)(4);
    filter2D(srcImage, resultImage, srcImage.depth(), kern);
	return resultImage;
}
int main()
{
	cv::Mat srcImage = cv::imread("..\\images\\flower3.jpg"); 
	if(!srcImage.data)
		return 0;	
	cv::Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	imshow("srcGray", srcGray);
	cv::Mat resultImage = Myfilter2D(srcGray);
	imshow("resultImage", resultImage);
	cv::Mat resultImage2 = filter2D_(srcGray);
	imshow("resultImage2", resultImage2);
	cv::waitKey(0);	 
	return 0;
}
