// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-4_doubleThreshold.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 双阈值化
# @Create Time: 2018-09-11 10:28:29
# @Last Modified: 2018-09-11 10:28:29
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
    // 初始化自适应阈值参数
    const int maxVal = 255;
    int low_threshold = 150;
    int high_threshold = 210;
    Mat dstTempImg1, dstTempImg2, dstImg;
    // 小阈值对源灰度图像进行阈值化操作
    threshold(srcGray, dstTempImg1, low_threshold, maxVal, THRESH_BINARY);
    // 大阈值对源灰度图像进行阈值化操作
    threshold(srcGray, dstTempImg2, high_threshold, maxVal, THRESH_BINARY_INV);
    // 矩阵与运算得到二值化结果
    bitwise_and(dstTempImg1, dstTempImg2, dstImg);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
