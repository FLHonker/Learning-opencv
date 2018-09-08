// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-18_mouseEvent.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 鼠标事件，显示坐标RGB信息
# @Create Time: 2018-09-08 15:44:49
# @Last Modified: 2018-09-08 15:44:49
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;

// 全局变量
Mat srcImg; // 原图像
void MouseEvent(int event, int x, int y, int flags, void* data)
{
    char charText[30];
    Mat tempImg, hsvImg;
    tempImg = srcImg.clone();
    // 左键按下移动显示RGB信息
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        Vec3b p = tempImg.at<Vec3b>(y, x);
        sprintf(charText, "R=%d, G%d, B=%", p[2], p[1], p[0]);
        // 写RGB信息文本到图像
        putText(tempImg, charText, Point(8,20), FONT_HERSHEY_PLAIN, 2, CV_RGB(0, 0, 255));
        imwrite("../out/RGBInfo.jpg", tempImg);
    }
    // 右键按下移动显示HSV信息
    else if(event == CV_EVENT_RBUTTONDOWN)
    {
        // 转换成HSV图像
        cvtColor(tempImg, hsvImg, CV_BGR2HSV);
        Vec3b p = hsvImg.at<Vec3b>(y, x);
        // 获取相应的通道值
        sprintf(charText, "H=%d, S=%d, V=%d", p[0], p[1], p[2]);
        // 写HSV文本信息到图像
        putText(tempImg, charText, Point(8, 20), FONT_HERSHEY_PLAIN, 2, CV_RGB(0, 255, 0));
        imwrite("../out/HSVInfo.jpg", tempImg);
    }
    // 没有按下键时显示对应坐标
    else 
    {
        // 移动鼠标显示坐标
        sprintf(charText, "x=%d, y=%d", x, y);
        // 写x，y坐标文本信息到图像
        putText(tempImg, charText, Point(8, 20), FONT_HERSHEY_PLAIN, 2, CV_RGB(255, 0, 0));
        imwrite("../out/xyInfo.jpg", tempImg);
    }
    imshow("srcImage", tempImg);   //show
}

int main(int argc, char** argv)
{
    // 读取图像及判断是否正确读入
   srcImg = imread("../../imgdata/apple.jpg");
    if(srcImg.empty())
        return -1;
    // 回调事件响应
    namedWindow("srcImage");
    setMouseCallback("srcImage", MouseEvent, 0);
    imshow("srcImage", srcImg);
    waitKey(0);
    return 0;
}
