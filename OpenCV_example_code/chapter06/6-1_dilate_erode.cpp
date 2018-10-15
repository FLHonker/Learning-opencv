// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 6-1_dilate_erode.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 膨胀与腐蚀操作
# @Create Time: 2018-10-14 21:19:00
# @Last Modified: 2018-10-14 21:19:00
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/test.jpg");
    if(!srcImg.data)
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_RGB2GRAY);
    Mat segMat, dilMat, eroMat;
    // 分离通道二值化
    inRange(srcGray, Scalar(0,0,100), Scalar(40, 30, 255), segMat);
    // 定义结构元素
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
    // 膨胀操作
    dilate(segMat, dilMat, element);
    // 腐蚀操作
    erode(segMat, eroMat, element);
    imshow("srcGray", srcGray);
    imshow("segMat", segMat);
    imshow("dilMat", dilMat);
    imshow("eroMat", eroMat);
    waitKey(0);
    return 0;
}
