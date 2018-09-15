// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-8_BGR-Histogram.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: BGR直方图
# @Create Time: 2018-09-14 17:24:17
# @Last Modified: 2018-09-14 17:24:17
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // 图像获取及判断
    Mat srcImg = imread("../../imgdata/apple.jpg");
    if(!srcImg.data)
        return -1;
    imshow("srcImage", srcImg);
    // 分离B、G、R三个通道
    vector<Mat> bgr_planes;
    split(srcImg, bgr_planes);
    // 初始化直方图参数
    int histSize = 256; 
    float range[] = { 0, 256 };
    const float* histRange[] = { range }; 
    bool uniform = true, accum = false;
    Mat b_hist, g_hist, r_hist;
    // 计算各个通道的直方图
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, 
             &histSize, histRange, uniform, accum);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, 
             &histSize, histRange, uniform, accum);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, 
             &histSize, histRange, uniform, accum);
    // 设置直方图绘图参数
    int hist_w = 640, hist_h = 512;
    int bin_w = cvRound(double(hist_w/histSize));
    Mat histImg(hist_h, hist_w, CV_8UC3, Scalar(0,0,0));
    // 分别归一化直方图到[0, histImg.rows]
    normalize(b_hist, b_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    // 分别对每个通道绘图
    for(int i = 0; i < histSize; ++i)
    {
        // 绘制B通道的直方图信息
        line(histImg, Point(bin_w*(i-1), hist_h-cvRound(b_hist.at<float>(i-1))),
             Point(bin_w*i, hist_h-cvRound(b_hist.at<float>(i))), Scalar(255,0,0), 2, 8, 0);
        // 绘制G通道的直方图信息
        line(histImg, Point(bin_w*(i-1), hist_h-cvRound(g_hist.at<float>(i-1))),
             Point(bin_w*i, hist_h-cvRound(g_hist.at<float>(i))), Scalar(0,255,0), 2, 8, 0);
        // 绘制R通道的直方图信息
        line(histImg, Point(bin_w*(i-1), hist_h-cvRound(r_hist.at<float>(i-1))),
             Point(bin_w*i, hist_h-cvRound(r_hist.at<float>(i))), Scalar(0,0,255), 2, 8, 0);
    }
    imshow("BGR-histImage", histImg);
    waitKey(0);
    return 0;
}
