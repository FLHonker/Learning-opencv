// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-28_HoughCircles_color.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 颜色圆检测
# @Create Time: 2018-10-14 12:44:14
# @Last Modified: 2018-10-14 12:44:14
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
    Mat srcImg = imread("../../imgdata/smarties.png");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    Mat resultImg = srcImg.clone();
    // 中值滤波
    medianBlur(srcImg, srcImg, 3);
    // 转换成HSV颜色空间
    Mat hsvImg;
    cvtColor(srcImg, hsvImg, COLOR_BGR2HSV);
    imshow("hsvImage", hsvImg);
    // 颜色阈值化处理
    Mat lowTempMat, upperTempMat;
    // 低阈值限定
    inRange(hsvImg, Scalar(0,100,100), Scalar(10,255,255), lowTempMat);
    // 高阈值限定
    inRange(hsvImg, Scalar(160,100,100), Scalar(179,255,255), upperTempMat);
    imshow("lowTempMat", lowTempMat);
    imshow("upperTempMat", upperTempMat);
    // 颜色阈值限定合并
    Mat redTempMat;
    addWeighted(lowTempMat, 1.0, upperTempMat, 1.0, 0.0, redTempMat);
    // 高斯平滑
    GaussianBlur(redTempMat, redTempMat, Size(9,9), 2, 2);
    // 藿夫变换
    vector<Vec3f> circles;
    HoughCircles(redTempMat, circles, CV_HOUGH_GRADIENT, 1, redTempMat.rows / 8, 100, 20, 0, 0);
    // 颜色圆检测结果判断
    if(circles.size() == 0)
        return 1;
    for(int i = 0; i < circles.size(); ++i)
    {
        // 绘制检测到的颜色圆
        Point center(round(circles[i][0]), round(circles[i][1]));
        int radius = round(circles[i][2]);
        circle(resultImg, center, radius, Scalar(0,255,0), 5);
    }
    imshow("resultImage", resultImg);
    waitKey(0);
    return 0;
}
