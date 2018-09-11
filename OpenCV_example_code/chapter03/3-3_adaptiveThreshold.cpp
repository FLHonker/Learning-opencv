// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-3_adaptiveThreshold.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: adaptiveThreshold自适应阈值
# @Create Time: 2018-09-11 09:54:29
# @Last Modified: 2018-09-11 09:54:29
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    // 图像读取及判断
    Mat srcImg = imread("../../imgdata/baboon.jpg");
    if(!srcImg.data)
        return -1;
    // 灰度转换
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    Mat dstImg;
    // 初始化自适应阈值参数
    int blockSize = 5;
    int constValue = 10;
    const int maxVal = 255;
    // 自适应阈值算法：
    // 0: ADAPTIVE_THRESH_MEAN_C
    // 1: ADAPTIVE_THRESH_GAUSSION_C
    // 阈值类型：
    // 0: THRESH_BINARY
    // 1: THRESH_BINARY_INV
    int adaptiveMethod = 0;
    int thresholdType = 1;
    // 图像自适应阈值操作
    adaptiveThreshold(srcGray, dstImg, maxVal, adaptiveMethod, thresholdType, blockSize, constValue);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
