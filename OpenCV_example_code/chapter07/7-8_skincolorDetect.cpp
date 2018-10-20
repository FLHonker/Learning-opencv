// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-8_skincolorDetect.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 肤色检测
# @Create Time: 2018-10-20 10:24:11
# @Last Modified: 2018-10-20 10:24:11
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
    Mat srcImg = imread("../../imgdata/hand.jpg");
    if(srcImg.empty())
        return -1;
    // 构建椭圆模型
    Mat skinMat = Mat::zeros(Size(256, 256), CV_8UC1);
    ellipse(skinMat, Point(113, 155.6), Size(23.4, 15.2),43.0, 0.0, 360.0, Scalar(255,255,255), -1);
    // 定义结构元素
    Mat struElement = getStructuringElement(MORPH_RECT, Size(3,3), Point(-1,-1));
    Mat YcrcbMat;
    Mat tempMat = Mat::zeros(srcImg.size(), CV_8UC1);
    // 颜色空间转换YCrCb
    cvtColor(srcImg, YcrcbMat, CV_BGR2YCrCb);
    // 椭圆皮肤模型检测
    for(int i = 0; i < srcImg.rows; ++i)
    {
        uchar* p = (uchar*)tempMat.ptr<uchar>(i);
        Vec3b* ycrcb = (Vec3b*)YcrcbMat.ptr<Vec3b>(i);
        for(int j = 0; j < srcImg.cols; ++j)
        {
            // 颜色判断
            if(skinMat.at<uchar>(ycrcb[j][1], ycrcb[j][2]) > 0)
                p[j] = 255;
        }
    }
    // 形态学闭操作
    morphologyEx(tempMat, tempMat, MORPH_CLOSE, struElement);
    // 定义轮廓参数
    vector<vector<Point>> contours, dstContours;
    vector<Vec4i> hierarchy;
    // 查找连通域
    findContours(tempMat, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    // 筛选伪轮廓
    for(size_t i = 0; i < contours.size(); ++i)
    {
        // 判断区域面积
        if(fabs(contourArea(Mat(contours[i]))) > 1000)
            dstContours.push_back(contours[i]);
    }
    tempMat.setTo(0);
    // 绘制轮廓
    drawContours(tempMat, dstContours, -1, Scalar(255, 0, 0), CV_FILLED);
    Mat dstImg;
    srcImg.copyTo(dstImg, tempMat);
    imshow("srcImage", srcImg);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
