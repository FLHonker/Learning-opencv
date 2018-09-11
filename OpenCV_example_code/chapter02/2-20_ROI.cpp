// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-20_ROI.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Rect选择感兴趣区域Regions of intresting
# @Create Time: 2018-09-09 09:44:29
# @Last Modified: 2018-09-09 09:44:29
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// 全局变量源图像
Mat srcImg;
// 方法1：利用Rect选择区域(100,180,150,50)
void regionExtraction(int xRoi, int yRoi, int widthRoi, int heightRoi)
{
    // 获取指定坐标区域
    Mat roiImg(srcImg.rows, srcImg.cols, CV_8UC3);
    cout << srcImg.rows << " x " << srcImg.cols <<endl;
    //将兴趣区域复制到目标图像
    srcImg(Rect(xRoi, yRoi, widthRoi, heightRoi)).copyTo(roiImg);
    imshow("roiImage", roiImg);
    waitKey(0);
}

int main()
{
    // 源图像载入及判断
    srcImg = imread("../../imgdata/butterfly.jpg");
    if(!srcImg.data)
        return -1;
    imshow("RoIing", srcImg);
    waitKey(0);
    // 方法1：利用Rect选择区域(100,180,150,50)
    int xRoi = 100, yRoi = 180, widthRoi = 150, heightRoi = 50;
    regionExtraction(xRoi, yRoi, widthRoi, heightRoi);
    return 0;
}
