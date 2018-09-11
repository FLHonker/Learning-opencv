// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-22_mouseROI_2.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 利用鼠标获取感兴趣区域2
# @Create Time: 2018-09-09 11:02:18
# @Last Modified: 2018-09-09 11:02:18
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
bool eventFlag = false;

void MouseEvent(int event, int x, int y, int flags, void* data)
{
    Mat tempImg = srcImg.clone();
    // 根据标志位判断当前建的有效性
    if(eventFlag)
    {
        // 计算选取区域的大小
        roiRect.x = MIN(startPoint.x, x);
        roiRect.width = abs(x - startPoint.x);
        roiRect.height = abs(y - startPoint.y);
    }
    // 鼠标按下操作
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        // 鼠标左键按下，起始点获取
        eventFlag = true;
        startPoint = Point(x, y);
        roiRect = Rect(x, y, 0, 0);
    }
    // 鼠标抬起操作
    else if(event == CV_EVENT_LBUTTONUP)
    {
        eventFlag = false;
        // 画出矩形框
        rectangle(tempImg, roiRect, Scalar(255,0,0), 2, 3, 0);
        imshow("ROIing", tempImg);
        imshow("ROI", tempImg(roiRect));
    }
    //waitKey(0);
}

int main()
{
    // 源图像载入及判断
    srcImg = imread("../../imgdata/butterfly.jpg");
    if (!srcImg.data)
        return -1;
    namedWindow("ROIing");
    imshow("ROIing", srcImg);
    // 方法3：利用鼠标左右键标志位控制选择区域2
    setMouseCallback("ROIing", MouseEvent, 0);
    waitKey(0);
    return 0;
}
