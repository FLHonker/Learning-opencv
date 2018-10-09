// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-7_Laplace.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 基本边缘检测算子——Laplace
# @Create Time: 2018-10-08 21:35:52
# @Last Modified: 2018-10-08 21:35:52
***********************************************/
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
       return -1;
    // 高斯平滑
    GaussianBlur(srcImg, srcImg, Size(3,3), 0, 0, BORDER_DEFAULT);
    Mat dstImg;
    // Laplace变换
    Laplacian(srcImg, dstImg, CV_16S, 3);
    convertScaleAbs(dstImg, dstImg);
    imshow("srcImage", srcImg);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
