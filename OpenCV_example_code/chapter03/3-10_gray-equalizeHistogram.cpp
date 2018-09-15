// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-10_gray-equalizeHistogram.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 灰度直方图均衡
# 核心：把集中灰度分布映射成灰度均匀分布；
# 目的是：均衡利用动态范围内的所有灰度阶。
# 直方图均衡化的步骤：
# (1) 计算输入图像的直方图；
# (2) 求累计分布直方图，构建查找表；
# (3) 通过图像映射，计算新的图像像素分布。
# @Create Time: 2018-09-14 21:40:50
# @Last Modified: 2018-09-14 21:40:50
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/apple.jpg");
    if(!srcImg.data)
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    // 直方图均衡化
    Mat heqResult;
    equalizeHist(srcGray, heqResult);
    imshow("heqResult", heqResult);
    waitKey(0);
    return 0;
}
