// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-8_Roberts.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 基本边缘检测算子——Roberts
# @Create Time: 2018-10-08 21:56:14
# @Last Modified: 2018-10-08 21:56:14
***********************************************/
#include <opencv2/opencv.hpp>

using namespace cv;

// Roberts算子实现
Mat roberts(Mat srcImg)
{
    Mat dstImg = srcImg.clone();
    int nRows = dstImg.rows;
    int nCols = dstImg.cols;
    for(int i = 0; i < nRows - 1; ++i)
    {
        for(int j = 0; j < nCols - 1; ++j)
        {
            // 根据公式计算
            int t1 = (srcImg.at<uchar>(i, j) - srcImg.at<uchar>(i+1, j+1)) * 
                (srcImg.at<uchar>(i, j) - srcImg.at<uchar>(i+1, j+1));
            int t2 = (srcImg.at<uchar>(i+1, j) - srcImg.at<uchar>(i, j+1)) * 
                (srcImg.at<uchar>(i+1, j) - srcImg.at<uchar>(i, j+1));
            // 计算对角线像素差
            dstImg.at<uchar>(i, j) = (uchar)sqrt(t1 + t2);
        }
    }
    return dstImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    Mat dstImg = roberts(srcImg);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
