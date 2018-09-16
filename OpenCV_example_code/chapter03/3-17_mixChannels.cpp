// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-17_mixChannels.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: mixChannels应用
# @Create Time: 2018-09-16 16:39:37
# @Last Modified: 2018-09-16 16:39:37
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

int main()
{
    Mat rgba(24, 24, CV_8UC4, Scalar(1,2,3,4));
    Mat bgr(rgba.rows, rgba.cols, CV_8UC3);
    Mat alpha(rgba.rows, rgba.cols, CV_8UC1);
    // 对数据头进行复制
    Mat out[] = { bgr, alpha };
    // 通道转换rgba[0]->bgr[0], rgba[1]->bgr[1]
    // rgba[2]->bgr[2], rgba[3]->alpha[0]
    int from_to[] = { 0,2, 1,1, 2,0, 3,3 };  // 偶数代表输入矩阵，奇数代表输出矩阵
    mixChannels(&rgba, 1, out, 2, from_to, 4);
    std::cout << out[0] << std::endl << out[1] << std::endl;
    return 0;
}
