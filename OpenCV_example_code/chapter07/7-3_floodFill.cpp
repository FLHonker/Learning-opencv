// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-3_floodFill.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: floodFill分割
# @Create Time: 2018-10-18 14:48:18
# @Last Modified: 2018-10-18 14:48:18
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 初始化参数
Mat image, gray, mask;
int ffillMode = 1;
int loDiff = 20, upDiff = 20;
int connectivity = 4;
bool isColor = true, useMask = false;
int newMaskVal = 255;

// 鼠标响应函数
static void onMouse(int event, int x, int y, int, void*)
{
    if(event != CV_EVENT_LBUTTONDOWN)
        return;
    // floodFill参数设置
    Point seed(x, y);
    int lo = ffillMode == 0 ? 0 : loDiff;
    int up = ffillMode == 0 ? 0 : upDiff;
    int flags = connectivity + (newMaskVal << 8) + 
        (ffillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
    // 颜色分量随机选取
    int b = (unsigned)theRNG() & 255;
    int g = (unsigned)theRNG() & 255;
    int r = (unsigned)theRNG() & 255;
    Rect ccomp;
    // 颜色选择
    Scalar newVal = isColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);
    Mat dst = isColor ? image : gray;
    int area;
    // 根据标志位选择洪泛填充
    if(useMask)
    {
        threshold(mask, mask, 1, 128, CV_THRESH_BINARY);
        area = floodFill(dst, mask, seed, newVal, &ccomp, Scalar(lo,lo,lo), Scalar(up,up,up), flags);
        imshow("mask", mask);
    }
    else
    {
        // 泛洪填充
        area = floodFill(dst, seed, newVal, &ccomp, Scalar(lo,lo,lo), Scalar(up,up,up), flags);
    }
    imshow("image", image);
}

int main()
{
    Mat srcImg = imread("../../imgdata/sea.jpg");
    if(srcImg.empty())
        return -1;
    srcImg.copyTo(image);
    cvtColor(srcImg, gray, CV_BGR2GRAY);
    mask.create(srcImg.rows + 2, srcImg.cols + 2, CV_8UC1);
    // 鼠标响应回调函数
    namedWindow("image", 0);
    setMouseCallback("image", onMouse, 0);
    for(;;)
    {
        imshow("image", isColor ? image : gray);
        int c = waitKey(0);
        // 按下ESC键退出
        if(c == 27)
        {
            cout << "Exiting..." <<endl;
            break;
        }
        // 按下R键重新进行图像分割
        if(c == 'r')
        {
            cout << "Original image is restored." <<endl;
            srcImg.copyTo(image);
            cvtColor(image, gray, CV_BGR2GRAY);
            mask = Scalar::all(0);
        }
    }
    return 0;
}
