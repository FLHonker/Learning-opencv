// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-6_cvSobel.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: OpenCV自带库计算图像边缘Sobel
# @Create Time: 2018-10-08 20:53:34
# @Last Modified: 2018-10-08 20:53:34
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg");
    if(srcImg.empty())
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_RGB2GRAY);
    imshow("srcGray", srcGray);
    // 定义边缘图，水平及垂直
    Mat edgeMat, edgeXMat, edgeYMat;
    // 求x方向Sobel边缘
    Sobel(srcGray, edgeXMat, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    // 求y方向Sobel边缘
    Sobel(srcGray, edgeYMat, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    // 线性变换，转换输入数组元素为8位无符号整型
    convertScaleAbs(edgeXMat, edgeXMat);
    convertScaleAbs(edgeYMat, edgeYMat);
    // x与y方向边缘叠加
    addWeighted(edgeXMat, 0.5, edgeYMat, 0.5, 0, edgeMat);
    imshow("edgeYMat", edgeYMat);
    imshow("edgeMat", edgeMat);
    // 定义Scharr边缘图像
    Mat edgeMatS, edgeXMatS, edgeYMatS;
    // 计算x方向Scharr边缘
    Scharr(srcGray, edgeXMatS, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(edgeXMatS, edgeXMatS);
    // 计算y方向Scharr边缘
    Scharr(srcGray, edgeYMatS, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(edgeYMatS, edgeYMatS);
    addWeighted(edgeXMatS, 0.5, edgeYMatS, 0.5, 0, edgeMatS);
    imshow("edgeMatS", edgeMatS);
    waitKey(0);
    return 0;
}
