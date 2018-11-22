// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-10_dynamic_carDetect.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 运动目标检测
# @Create Time: 2018-11-22 11:27:13
# @Last Modified: 2018-11-22 11:27:13
***********************************************/
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace cv;

Ptr<BackgroundSubtractorMOG2> pBackgroundMOG2;
// MOG2目标建模检测
vector<Rect> get_foreground_objects(Mat scene, double scale, bool bFlag)
{
    // 标志位控制当前检测图像是否开启背景建模
    if(bFlag == false)
    {
        vector<Rect> one_rect;
        Rect whole;
        whole.x = whole.y = 0;
        whole.width = scene.cols;
        whole.height = scene.rows;
        one_rect.push_back(whole);
        return one_rect;
    }
    Mat img;
    // 图像尺寸缩放
    resize(scene, img, Size(0,0), scale, scale);
    Mat fgMask, fgImg, bgImg;
    // 更新背景模型
    pBackgroundMOG2->apply(img, fgMask);
    // 中值滤波
    medianBlur(fgMask, fgMask, 5);
    imshow("fgMask", fgMask);
    // 形态学比操作
    morphologyEx(fgMask, fgMask, MORPH_CLOSE, Mat::ones(15,3,CV_8UC1));
    // 找到连通区域
    vector<vector<Point>> region_contours;
    findContours(fgMask, region_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    vector<Rect> objects;
    for(size_t i = 0; i < region_contours.size(); ++i)
    {
        // 计算连通域的最小外接矩形
        Rect rect = boundingRect(region_contours[i]);
        // 目标有效性区域判断
        rect.y += rect.height * 1/4;
        rect.height *= 3.0 / 4;
        // 对检测到的外接矩形区域进行缩放
        rect.x /= scale;
        rect.y /= scale;
        rect.width /= scale;
        rect.height /= scale;
        // 区域形态满足基本条件
        if(rect.area() > scene.total()/400)
            objects.push_back(rect);
    }
    return objects;
}

int main(int argc, char** argv)
{
    // 创建MOG2背景建模类
    pBackgroundMOG2 = createBackgroundSubtractorMOG2();
    // 视频读取
    VideoCapture capture("../../imgdata/car.avi");
    if(!capture.isOpened())
        exit(EXIT_FAILURE);
    Mat frame;
    vector<Rect> regions;
    while((char)waitKey(1) != 27)
    {
        // 读取当前帧
        if(!capture.read(frame))
            exit(EXIT_FAILURE);
        // 缩小图像尺寸，检测目标
        regions = get_foreground_objects(frame, 0.25, true);
        for(int i = 0; i < regions.size(); ++i)
        {
            // 疑似窗口区域显示
            Mat regionImg;
            regionImg = frame(regions[i]);
            imshow("target object", regionImg);
        }
    }
    return 0;
}
