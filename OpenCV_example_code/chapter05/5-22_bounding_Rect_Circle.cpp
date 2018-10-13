// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-22_bounding_Rect_Circle.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 轮廓边界矩形与圆
# @Create Time: 2018-10-12 10:58:45
# @Last Modified: 2018-10-12 10:58:45
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

// 计算外接矩形与圆轮廓
void cacBounding(Mat src)
{
    RNG rng(12345);
    Mat threMat;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // 二值化操作
    threshold(src, threMat, 120, 255, THRESH_BINARY);
    imshow("threMat", threMat);
    // 计算边界轮廓
    findContours(threMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 多边形逼近轮廓，获取矩形和圆形边界框
    vector< vector<Point> > conPoint(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());
    for(int i = 0; i < contours.size(); ++i)
    {
        // 多边形近似
        approxPolyDP(Mat(contours[i]), conPoint[i], 3, true);
        // 计算最小外接矩形
        boundRect[i] = boundingRect(Mat(contours[i]));
        // 生成最小封闭圆
        minEnclosingCircle(conPoint[i], center[i], radius[i]);
    }
    // 显示绘图
    Mat drawing = Mat::zeros(threMat.size(), CV_8UC3);
    for(int i = 0; i < contours.size(); ++i)
    {
        // 随机生成颜色
        Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
        // 绘制多边形轮廓
        drawContours(drawing, conPoint, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        // 绘制多边形包围的矩形框
        rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
        // 绘制多边形圆形框
        circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
    }
    imshow("boundingRect", drawing);
}

// 计算可倾斜外接矩形与椭圆轮廓
void cacBoundRotatedRect(Mat src)
{
    RNG rng(12345);
    Mat threMat;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // 二值化操作
    threshold(src, threMat, 120, 255, THRESH_BINARY);
    // 计算边界轮廓
    findContours(threMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 对每个找到的轮廓创建可倾斜的边界框和圆
    vector<RotatedRect> minRect(contours.size());
    vector<RotatedRect> minEllipse(contours.size());
    for(int i = 0; i < contours.size(); ++i)
    {
        // 计算最小面积矩形
        minRect[i] = minAreaRect(Mat(contours[i]));
        // 求外接轮廓的椭圆
        if(contours[i].size() > 5)
            minEllipse[i] = fitEllipse(Mat(contours[i]));
    }
    // 绘出轮廓及其可倾斜的边界框和边界椭圆
    Mat drawing = Mat::zeros(threMat.size(), CV_8UC3);
    for(int i = 0; i < contours.size(); ++i)
    {
        // 随机生成颜色
        Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
        // 绘制边界轮廓
        drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        // 绘制边界椭圆
        ellipse(drawing, minEllipse[i], color, 2, 8);
        // 绘制边界矩形
        Point2f rect_points[4];
        minRect[i].points(rect_points);
        for(int j = 0; j < 4; ++j)
            line(drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8);
    }
    imshow("boundRotatedRect", drawing);
}

int main()
{
    Mat srcImg = imread("../../imgdata/shapes1.jpg");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    // 转换成灰度图并进行平滑
    Mat srcGray, blurMat;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    blur(srcGray, blurMat, Size(3,3));
    imshow("blurMat", blurMat);
    // 计算外接矩形与圆轮廓
    cacBounding(blurMat);
    // 计算可倾斜外接矩形与椭圆轮廓
    cacBoundRotatedRect(blurMat);
    waitKey(0);
    return 0;
}
