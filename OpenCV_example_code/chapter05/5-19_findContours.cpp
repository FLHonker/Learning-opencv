// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-19_findContours.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 轮廓检测
# @Create Time: 2018-10-11 11:09:20
# @Last Modified: 2018-10-11 11:09:20
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat srcGray;
int thresh = 100, max_thresh = 255;
RNG rng(12345);

// mouse callback event 
void thresh_callback(int, void*)
{
    Mat canny_output;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // 用Canny算子检测边缘
    Canny(srcGray, canny_output, thresh, thresh * 2, 3);
    // 寻找轮廓
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 绘制轮廓
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for(int i = 0; i < contours.size(); ++i)
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        // 随机颜色绘制轮廓
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
    }
    // 显示轮廓结果
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
}

int main()
{
    Mat srcImg = imread("../../imgdata/shapes2.jpg");
    if(srcImg.empty())
        return -1;
    // 转换灰度并平滑
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    blur(srcGray, srcGray, Size(3,3));
    // 创建窗体
    namedWindow("srcImage", CV_WINDOW_AUTOSIZE);
    imshow("srcImage", srcImg);
    // 滑动条控制Canny阈值
    createTrackbar(" thresh", "srcImage", &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);
    waitKey(0);
    return 0;
}
