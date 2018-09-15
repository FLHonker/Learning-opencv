// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-7_H-S-Histogram.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: H-S直方图（色相饱和度直方图）
# @Create Time: 2018-09-12 21:25:36
# @Last Modified: 2018-09-12 21:25:36
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat srcImg, hsvMat;
    srcImg = imread("../../imgdata/fruits.jpg");
    if(srcImg.empty())
        return -1;
    cvtColor(srcImg, hsvMat, CV_BGR2HSV);
    // 初始化灰度阶参数
    int hbins = 30, sbins = 32;
    int histSize[] = { hbins, sbins };
    // 灰度变化范围设置
    float hranges[] = { 0, 180 };
    // 饱和度变化范围
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // 选取计算直方图通道
    int channels[] = { 0, 1 };
    // 计算当前通道直方图
    calcHist(&hsvMat, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
    double maxVal = 0;
    // 找到直方图的最大值
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);
    // 遍历H、S通道
    for(int h = 0; h < hbins; ++h)
    {
        for(int s = 0; s < sbins; ++s)
        {
            float binVal = hist.at<float>(h,s);
            // 根据最大值计算变换范围
            int intensity = cvRound(binVal * 255 / maxVal);
            // 绘图显示
            rectangle(histImg, Point(h*scale, s*scale), Point((h+1)*scale-1, 
                    (s+1)*scale-1), Scalar::all(intensity), CV_FILLED);
        }
    }
    imshow("srcImage", srcImg);
    imshow("H-S Histogram", histImg);
    waitKey(0);
    return 0;
}
