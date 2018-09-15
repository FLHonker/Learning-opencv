// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-11_color-equalizeHistogram.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 彩色直方图均衡化
# 实现RGB颜色空间下的图像细节增强，常应用于图像增强、自动白平衡和伪彩色图像处理
# @Create Time: 2018-09-14 22:55:25
# @Last Modified: 2018-09-14 22:55:25
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

int main()
{ 
    Mat srcImg = imread("../../imgdata/butterfly.jpg");
    if(!srcImg.data)
        return -1;
    // 存储彩色直方图及图像通道向量
    Mat colorHeqImg;
    std::vector<Mat> BGR_plane;
    // 对BGR通道进行分离
    split(srcImg, BGR_plane);
    // 分别对BGR进行直方图均衡化
    for(int i = 0; i < BGR_plane.size(); ++i)
        equalizeHist(BGR_plane[i], BGR_plane[i]);
    // 合并对应的各个通道
    merge(BGR_plane, colorHeqImg);
    imshow("srcImage", srcImg);
    imshow("colorHeqImage", colorHeqImg);
    waitKey(0);
    return 0;
}
