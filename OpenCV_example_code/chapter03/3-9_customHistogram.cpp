// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-9_customHistogram.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 自定义直方图
# @Create Time: 2018-09-14 19:52:41
# @Last Modified: 2018-09-14 19:52:41
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    // 图像读取及判断
    Mat srcImg = imread("../../imgdata/apple.jpg");
    if(!srcImg.data)
        return -1;
    imshow("srcImage", srcImg);
    // 灰度转换
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 初始化直方图统计参数
    const int channels[] = { 0 };
    const int histSize[] = { 256 };
    // 设定区间[0,60], [61, 120], [121, 160], [161, 220], [221, 255]
    float hrange[] = {0, 60, 120, 160, 220, 255};
    const float* ranges[] = { hrange };
    // 计算直方图
    MatND hist;
    calcHist(&srcGray, 1, channels, Mat(), hist, 1, histSize, ranges);
    // 求直方图中最大值
    double maxHist = 0;
    minMaxLoc(hist, 0, &maxHist, 0, 0);
    // 设置直方图中的最大值
    int hist_size = hist.rows;
    Mat histImg(hist_size, hist_size, CV_8U, Scalar(255));
    // 绘制直方图
    for(int h = 0; h < hist_size; ++h)
    {
        float binVal = hist.at<float>(h);
        // 归一化，根据最大值计算变化范围
        int intensity = static_cast<int>(binVal * hist_size / maxHist);
        // 绘制直方图信息
        line(histImg, Point(h, hist_size), Point(h, hist_size-intensity), Scalar::all(0));
    }
    imshow("myHistImage", histImg);
    waitKey(0);
    return 0;
}
