// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-27_Shi-Tomasi.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Shi-Tomasi角点检测
# @Create Time: 2018-10-13 17:29:54
# @Last Modified: 2018-10-13 17:29:54
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
    Mat srcImg = imread("../../imgdata/shapes1.jpg");
    if(srcImg.empty())
        return -1;
    Mat srcGray;
    Mat resultImg = srcImg.clone();
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 设置角点检测参数
    vector<Point2f> vecCorners;
    // 可允许接受的角点最差质量
    double qualityLevel = 0.01;
    // 角点间最小的欧氏距离
    double minDistance = 10;
    // 像素邻域中计算协方差矩阵窗口的尺寸
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    // 检测到的角点输出的最大数目
    int maxCorners = 50;
    int maxTrackbar = 100;
    // 调用函数进行Shi-Tomasi角点检测
    goodFeaturesToTrack(srcGray, vecCorners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
    cout << "Corners:" << vecCorners.size() <<endl;
    // 绘制检测角点
    for(int i = 0; i < vecCorners.size(); ++i)
        circle(resultImg, vecCorners[i], 4, Scalar(0,0,255), 2);
    imshow("srcImage", srcImg);
    imshow("Shi-Tomasi", resultImg);
    waitKey(0);
    return 0;
}
