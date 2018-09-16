// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-19_distanceTransform.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 距离变换——distanceTransform
# @Create Time: 2018-09-16 17:37:23
# @Last Modified: 2018-09-16 17:37:23
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat srcImg  = imread("../../imgdata/rabbit.jpg");
    if(!srcImg.data)
        return -1;
    // 转换为灰度图
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 二值化
    Mat srcBin;
    threshold(srcGray, srcBin, 160, 255, THRESH_BINARY);
    // 距离变换
    Mat dstImg;
    distanceTransform(srcBin, dstImg, CV_DIST_L2, CV_DIST_MASK_PRECISE);
    // 归一化矩阵
    normalize(dstImg, dstImg, 0, 1.0, NORM_MINMAX);
    imshow("srcBinary", srcBin);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
