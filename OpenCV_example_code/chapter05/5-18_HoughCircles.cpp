// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-18_HoughCircles.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 藿夫圆变换——HoughCircles
# @Create Time: 2018-10-10 17:26:11
# @Last Modified: 2018-10-10 17:26:11
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/shapes2.jpg");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    // 转换为灰度图
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 高斯平滑滤波
    GaussianBlur(srcGray, srcGray, Size(9,9), 2, 2);
    std::vector<Vec3f> circles;
    HoughCircles(srcGray, circles, CV_HOUGH_GRADIENT, 1, srcGray.rows / 8, 200, 100, 0, 0);
    // 将得到的结果绘图
    for(size_t i = 0; i < circles.size(); ++i)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // 绘制圆心,半径为3
        circle(srcImg, center, 3, Scalar(0, 255, 0));
        // 绘制圆轮廓
        circle(srcImg, center, radius, Scalar(120, 120, 120), 3, 8, 0);
    }
    imshow("HoughCircles", srcImg);
    waitKey(0);
    return 0;
}
