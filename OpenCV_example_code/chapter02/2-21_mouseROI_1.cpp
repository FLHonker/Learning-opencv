// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-21_mouseROI_1.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 利用鼠标按键获取感兴趣区域1
# @Create Time: 2018-09-09 10:03:18
# @Last Modified: 2018-09-09 10:03:18
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 全局变量源图像
Mat srcImg;
// 所选矩形区域
Rect roiRect;
// 起始点、终止点
Point startPoint, endPoint;
// 完成所选区域标志位
bool downFlag = false, upFlag = false;

void MouseEvent(int event, int x, int y, int flags, void* data)
{
    // 鼠标左键按下，获取起始点
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        downFlag = true;
        startPoint.x = x;
        startPoint.y = y;
    }
    // 鼠标拖到松开，获取终止点
    if(event == CV_EVENT_LBUTTONUP)
    {
        upFlag = true;
        endPoint.x = x;
        endPoint.y = y;
    }
    // 显示拖到图像区域
    if(downFlag == true && upFlag == false)
    {
        Point tempPoint;
        tempPoint.x = x;
        tempPoint.y = y;
        // 用于显示图像生成
        Mat tempImg = srcImg.clone();
        rectangle(tempImg, startPoint, tempPoint, Scalar(255,0,0), 2, 3, 0);
        imshow("ROIing", tempImg);
    }
    // 选择区域生成
    if(downFlag == true && upFlag == true)
    {
        // 获取选择区域的ROI
        roiRect.width = abs(startPoint.x - endPoint.x);
        roiRect.height = abs(startPoint.y - endPoint.y);
        roiRect.x = min(startPoint.x, endPoint.x);
        roiRect.y = min(startPoint.y, endPoint.y);
        Mat roiImg(srcImg, roiRect);
        imshow("ROI", roiImg);
        downFlag = false;
        upFlag = false;
    }
    waitKey(0);
}

int main()
{
    // 源图像载入及判断
    srcImg = imread("../../imgdata/butterfly.jpg");
    if (!srcImg.data)
        return -1;
    namedWindow("ROIing");
    imshow("ROIing", srcImg);
    // 方法2：利用鼠标左右键标志位控制选择区域1
    setMouseCallback("ROIing", MouseEvent, 0);
    waitKey(0);
    return 0;
}
