// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-15_compareHist.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 直方图对比
# @Create Time: 2018-09-16 11:31:42
# @Last Modified: 2018-09-16 11:31:42
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;

int main(int argc, char** argv)
{
    Mat src_base, hsv_base;
    Mat src_test1, hsv_test1;
    Mat src_test2, hsv_test2;
    Mat hsv_half_down;
    // 加载不同环境下的三幅图像
    src_base = imread("../../imgdata/left08.jpg");
    src_test1 = imread("../../imgdata/aero1.jpg");
    src_test2 = imread("../../imgdata/apple.jpg");
    if(!src_base.data || !src_test1.data || !src_test2.data)
        return -1;
    imshow("HistComSrc0", src_base);
    imshow("HistComSrc1", src_test1);
    imshow("HistComSrc2", src_test2);
    waitKey(0);

    // 分别转换为HSV通道下的图像
    cvtColor(src_base, hsv_base, CV_BGR2HSV);
    cvtColor(src_test1, hsv_test1, CV_BGR2HSV);
    cvtColor(src_test2, hsv_test2, CV_BGR2HSV);
    hsv_half_down = hsv_base(Range(hsv_base.rows/2, hsv_base.rows-1),
                             Range(hsv_base.cols/2, hsv_base.cols-1));
    // hue通道设置为30bins，saturation设置为32bins
    int h_bins = 30, s_bins = 32;
    int histSize[] = { h_bins, s_bins };
    // 设置hue的取值从0到256，saturation的取值从0到180
    float h_ranges[] = { 0, 256 };
    float s_ranges[] = { 0, 180 };
    const float* ranges[] = { h_ranges, s_ranges };
    int channels[] = { 0, 1 };
    // 建立直方图
    MatND hist_base, hist_half_down, hist_test1, hist_test2;
    // 计算相应通道下的直方图
    calcHist(&hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
    normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());
    calcHist(&hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false);
    normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());
    calcHist(&hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
    normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());
    calcHist(&hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false);
    normalize(hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat());

    // 应用不同直方图对比方法
    for(int i = 0; i < 4; ++i)
    {
        int compare_method = i;
        double base_base = compareHist(hist_base, hist_base, compare_method);
        double base_half = compareHist(hist_base, hist_half_down, compare_method);
        double base_test1 = compareHist(hist_base, hist_test1, compare_method);
        double base_test2 = compareHist(hist_base, hist_test2, compare_method);
        printf("Method [%d] Perfect, Base-Half, Base-Test1, Base-Test2 : %f, %f, %f, %f \n",
               i, base_base, base_half, base_test1, base_test2);
    }
    return 0;
}
