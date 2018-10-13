// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-23_cacMoments.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 计算轮廓矩
# @Create Time: 2018-10-12 12:01:03
# @Last Modified: 2018-10-12 12:01:03
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

// 计算图像矩
void cacMoments(Mat src)
{
    Mat srcGray;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // 高斯滤波
    GaussianBlur(src, src, Size(3,3), 0.1, 0, BORDER_DEFAULT);
    // 灰度转换
    cvtColor(src, srcGray, CV_BGR2GRAY);
    // 轮廓边界检测
    findContours(srcGray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 绘制边界
    //drawContours(src, contours, -1, cvScalar(0,0,255));
    cout << "Number of contours: " << (int)contours.size() <<endl;
    // 计算轮廓矩
    vector<Moments> mu(contours.size());
    for(int i = 0; i < contours.size(); ++i)
        mu[i] = moments(contours[i], false);
    // 分析矩图像的相关特征
    for(int i = 0; i < contours.size(); ++i)
    {
        // 面积、重心、边界轮廓长度
        int area = mu[i].m00;
        int cx = mu[i].m10 / mu[i].m00;
        int cy = mu[i].m01 / mu[i].m00;
        int perimeter = arcLength(contours.at(i), true);
        // 椭圆
        if(contours[i].size() < 5)
            continue;
        RotatedRect rRect = fitEllipse(contours.at(i));
        double orientation = rRect.angle;
        double orientation_rads = orientation * CV_PI / 180;
        double majorAxis = rRect.size.height > rRect.size.width ? rRect.size.height : rRect.size.width;
        double minorAxis = rRect.size.height > rRect.size.width ? rRect.size.width : rRect.size.height;
        // 圆形度、离心率、周长、半径 
        double roundness = pow(perimeter, 2) / (2 * CV_PI * area);
        double eccentricity = sqrt(1 - pow(minorAxis / majorAxis, 2));
        double ratio = (minorAxis / majorAxis) * 100;
        double diameter = sqrt(4 * area / CV_PI);
        // 输出相关特征信息
        printf("面积：%d\n", area);
        printf("周长：%d\n", perimeter);
        printf("长轴：%.1f\n", majorAxis);
        printf("短轴：%.1f\n", minorAxis);
        printf("方向：%.1f\n", orientation);
        printf("圆形度：%.1f\n", roundness);
        printf("离心率：%.1f\n", eccentricity);
        printf("比例：%.1f\n", ratio);
        printf("直径：%.1f\n", diameter);
        printf("\n");
        // 绘制矩形及椭圆
        rectangle(src, boundingRect(contours[i]), cvScalar(0,0,255));
        ellipse(src, rRect, cvScalar(0,255,0));
        // 绘制相关坐标
        //line(src, Point(cx-30, cy), Point(cx+30, cy), cvScalar(0,0,255));
        //line(src, Point(cx, cy-30), Point(cx, cy+30), cvScalar(0,0,255));
        // 绘制起始线
        line(src, Point(cx, cy), Point(cx+30*cos(orientation_rads), cy+30*sin(orientation_rads)), cvScalar(255,0,0), 1);
        // 输出图像起始线
        char pChar[100];
        sprintf(pChar, "Ori: %.0f", orientation);
        putText(src, pChar, Point(cx,cy), FONT_HERSHEY_SIMPLEX, 0.4, cvScalar(255));
    }
    imshow("result", src);
}

int main()
{
    Mat srcImg = imread("../../imgdata/pic1.png");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    // 计算轮廓矩
    cacMoments(srcImg);
    waitKey(0);
    return 0;
}
