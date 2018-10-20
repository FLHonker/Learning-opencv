// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-5_Grabcut.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Grabcut图像分割
# @Create Time: 2018-10-19 10:55:48
# @Last Modified: 2018-10-19 10:55:48
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 全局变量
Point point1, point2;
int drag = 0;
Rect rect;
Mat srcImg, roiImg, rectImg;
bool select_flag = true;
vector<Point> Pf, Pb;

// 鼠标响应回调
void onMouse(int event, int x, int y, int flags, void* param)
{
    // 左键按下
    if(event == CV_EVENT_LBUTTONDOWN && !drag)
    {
        point1 = Point(x, y);
        drag = 1;
    }
    // 鼠标移动
    else if(event == CV_EVENT_MOUSEMOVE && drag)
    {
        Mat img1 = srcImg.clone();
        point2 = Point(x, y);
        // 绘制图像上的矩形区域
        rectangle(img1, point1, point2, CV_RGB(255,0,0), 3, 8, 0);
        imshow("image", img1);
    }
    // 左键抬起与拖拽标志
    else if(event == CV_EVENT_LBUTTONUP && drag)
    {
        point2 = Point(x, y);
        rect = Rect(point1.x, point1.y, x-point1.x, y-point1.y);
        drag = 0;roiImg = srcImg(rect);
    }
    // 右键按下
    else if(event == CV_EVENT_RBUTTONDOWN)
    {
        select_flag = false;
        drag = 0;
        imshow("ROI", roiImg);
    }
}

int main()
{
    srcImg = imread("../../imgdata/sea.jpg");
    if(srcImg.empty())
        return -1;
    // 定义前景与输出图像
    Mat srcImg2 = srcImg.clone();
    Mat foreground(srcImg.size(), CV_8UC3, Scalar(255,255,255));
    Mat result(srcImg.size(), CV_8UC1);
    // Grabcut分割前景与背景
    Mat fgMat, bgMat;
    namedWindow("srcImage", 1);
    imshow("srcImage", srcImg);
    // 迭代次数
    int i = 20;
    cout << "20 iters" <<endl;
    // 鼠标响应
    setMouseCallback("srcImage", onMouse, NULL);
    // 选择区域有效，按下Esc键退出
    while(select_flag == true && waitKey(0) != 27)
    {
        // 实现图割操作
        grabCut(srcImg, result, rect, bgMat, fgMat, i, GC_INIT_WITH_RECT);
        // 图像匹配
        compare(result, GC_PR_FGD, result, CMP_EQ);
        // 生成前景图像
        srcImg.copyTo(foreground, result);
        imshow("foreground", foreground);
        i--;
    }
    waitKey(0);
    return 0;
}
