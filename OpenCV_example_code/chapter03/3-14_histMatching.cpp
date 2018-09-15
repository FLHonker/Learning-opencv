// C9999999999999999999999999999999999999999999999999998/++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-14_histMatching.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 直方图匹配（直方图规格化）
# 步骤：
# (1) 分别计算源图像和目标图像的累计概率分布；
# (2) 分别对源图像和目标图像进行直方图均衡化操作；
# (3) 利用组映射关系使源图像直方图按照规定进行变换。
# @Create Time: 2018-09-15 20:37:31
# @Last Modified: 2018-09-15 20:37:31
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // 加载图像并显示
    Mat srcImg = imread("../../imgdata/fruits.jpg");
    Mat dstImg = imread("../../imgdata/blox.jpg");
    if(!srcImg.data || !dstImg.data)
        return -1;
    resize(dstImg, dstImg, Size(srcImg.rows, srcImg.cols), 0, 0, CV_INTER_LINEAR);
    imshow("srcImage", srcImg);
    Imshow("dstImage", dstImg);
    waitKey(0);

    // 初始化累计分布参数
    
}
