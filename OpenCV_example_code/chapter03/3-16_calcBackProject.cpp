// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-16_calcBackProject.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 直方图的反向投影
# @Create Time: 2018-09-16 13:28:24
# @Last Modified: 2018-09-16 13:28:24
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/board.jpg");
    if(!srcImg.data)
        return -1; 
    // 转换到HSV空间
    Mat hsvImg;
    cvtColor(srcImg, hsvImg, CV_BGR2HSV);
    // hue通道分离
    Mat hueImg;
    hueImg.create(hsvImg.size(), hsvImg.depth());
    int ch[] = { 0, 0 };
    mixChannels(&hsvImg, 1, &hueImg, 1, ch, 1);
    // 初始化直方图统计参数
    int bins = 25;
    MatND hist;
    int histSize = MAX(bins, 2);
    float hue_range[] = { 0, 100 };
    const float* ranges = { hue_range };
    // 计算直方图并归一化
    calcHist(&hueImg, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
    normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());
    // 计算反向投影
    MatND backproj;
    calcBackProject(&hueImg, 1, 0, hist, backproj, &ranges, 1, true);
    // 定义输出图像
    int w = 320, h = 360;
    int bin_w = cvRound((double)w/histSize);
    Mat histImg = Mat::zeros(w, h, CV_8UC3);
    for(int i = 0; i < bins; ++i)
    {
        // 绘制直方图
        rectangle(histImg, Point(i*bin_w, h), Point((i+1)*bin_w, h - cvRound(hist.at<float>(i)*h/255.0)), 
                  Scalar(0,0,255), -1);
    }
    // 显示源图像与反向投影图
    imshow("srcImage", srcImg);
    imshow("backproj", backproj);
    imshow("histogarm", histImg);
    // 直方图均衡化
    equalizeHist(backproj, backproj);
    imshow("backproj_equa", backproj);
    waitKey(0);
    return 0;
}   

