// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 6-3_morph_gradient.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 形态学梯度操作
# @Create Time: 2018-10-15 17:11:43
# @Last Modified: 2018-10-15 17:11:43
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/flower.jpg");
    if(srcImg.empty())
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 定义结构元素
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    // 形态学梯度
    Mat gradMat;
    morphologyEx(srcGray, gradMat, MORPH_GRADIENT, element);
    imshow("srcGray", srcGray);
    imshow("gradMat", gradMat);
    waitKey(0);
    return 0;
}
