// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-19_trackBarThreshold.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 利用createTrackBar进行二值化
# @Create Time: 2018-09-08 23:10:29
# @Last Modified: 2018-09-08 23:10:29
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// createTrackBar的回调响应函数
void onChangeTrackBar(int pos, void* data)
{
    // 强制类型转换
    Mat srcImg = *(Mat*)(data);
    Mat dstImg;
    // 根据滑动条的值进行二值化
    threshold(srcImg, dstImg, pos, 255, 0);
    imshow("dyn_threshold", dstImg);
}

int main()
{
    // 源图像载入及判断
    Mat srcImg = imread("../../imgdata/fruits.jpg");
    if(!srcImg.data)
        return -1;
    // 转为灰度图
    Mat gray;
    cvtColor(srcImg, gray, CV_RGB2GRAY);
    namedWindow("dyn_threshold");
    imshow("dyn_threshold", gray);
    // 创建滑动条createThreshold，调用回调函数
    createTrackbar("pos", "dyn_threshold", 0, 255, onChangeTrackBar, &gray);
    waitKey(0);
    return 0;
}
