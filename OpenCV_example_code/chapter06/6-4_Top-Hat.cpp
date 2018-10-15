// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 6-4_Top-Hat.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 形态学顶帽操作与黒帽操作
# src - 开运算结果图，闭运算结果图 - src 
# @Create Time: 2018-10-15 17:19:06
# @Last Modified: 2018-10-15 17:19:06
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
    Mat topHatMat, blackHatMat;
    // 形态学Top-Hat顶帽
    morphologyEx(srcGray, topHatMat, MORPH_TOPHAT, element);
    // 形态学Black-Hat黒帽
    morphologyEx(srcGray, blackHatMat, MORPH_BLACKHAT,element);
    imshow("topHatMat", topHatMat);
    imshow("blackHatMat", blackHatMat);
    waitKey(0);
    return 0;
}
