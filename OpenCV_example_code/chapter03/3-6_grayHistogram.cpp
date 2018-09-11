// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-6_grayHistogram.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 灰度直方图
# @Create Time: 2018-09-11 15:25:24
# @Last Modified: 2018-09-11 15:25:24
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    // 图像源读取及判断
    Mat srcImg, grayImg;
    srcImg = imread("../../imgdata/graf1.png");
    if(!srcImg.data)
        return -1;
    imshow("srcImage", srcImg);
    // 转换为灰度图
    cvtColor(srcImg, grayImg, CV_BGR2GRAY);
    // 定义直方图参数
    const int channels[1] = { 0 };
    const int histSize[1] = { 256 };
    float pranges[2] = { 0, 255 };
    const float* ranges[1] = { pranges };
    MatND hist;
    // 计算直方图
    // 输入源图像，图像源中的图个数，通道数，可选掩码参数，输出直方图，直方图维数，直方图横坐标区间数，每一组的上下界范围数组
    calcHist(&grayImg, 1, channels, Mat(), hist, 1, histSize, ranges);
    // 初始化画布参数
    int hist_w = 500, hist_h = 500;
    int nHistSize = 255;
    // 区间，cvRound()取最接近的整数值
    int bin_w = cvRound((double)hist_w / nHistSize);
    Mat histImg(hist_w, hist_h, CV_8SC3, Scalar(0,0,0));
    // 将直方图归一化到[0, histImg.rows]
    normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    // 在直方图画布上画出直方图
    for(int i = 1; i < nHistSize; ++i)
    {
        line(histImg, Point(bin_w * (i-1), hist_h - cvRound(hist.at<float>(i))), 
             Point(bin_w * i, hist_h - cvRound(hist.at<float>(i))), 
             Scalar(0,0,255), 2 ,8, 0);
    }
    // 显示直方图
    imshow("histImage", histImg);
    waitKey(0);
    return 0;
}
