// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-12-14_eyesDetect.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 人眼检测与跟踪
# @Create Time: 2018-11-23 14:19:36
# @Last Modified: 2018-11-23 14:19:36
***********************************************/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

CascadeClassifier face_cascade, eye_cascade;

// 人眼检测
int detectEye(Mat& im, Mat& tpl, Rect& rect)
{
    vector<Rect> faces, eyes;
    // 多尺度人脸检测
    face_cascade.detectMultiScale(im, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));
    // 遍历人脸检测结果，查找人眼目标
    for(int i = 0; i < faces.size(); ++i)
    {
        Mat face = im(faces[i]);
        // 多尺度人眼检测
        eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(20,20));
        // 人眼检测区域输出
        if(eyes.size())
        {
            rect = eyes[0] + Point(faces[i].x, faces[i].y);
            tpl = im(rect);
        }
    }
    return eyes.size();
}

// 人眼跟踪
void trackEye(Mat& im, Mat& tpl, Rect& rect)
{
    // 人眼位置
    Size pSize(rect.width * 2, rect.height * 2);
    // 矩形区域
    Rect tRect(rect + pSize - Point(pSize.width/2, pSize.height/2));
    tRect &= Rect(0, 0, im.cols, im.rows);
    // 生成匹配模板
    Mat tempMat(tRect.width - tpl.rows + 1, tRect.height - tpl.cols + 1, CV_32FC1);
    // 模板匹配
    matchTemplate(im(tRect), tpl, tempMat, CV_TM_SQDIFF_NORMED);
    // 计算最小、最大值
    double minval, maxval;
    Point minloc, maxloc;
    minMaxLoc(tempMat, &minval, &maxval, &minloc, &maxloc);
    // 区域检测判断
    if(minval <= 0.2)
    {
        rect.x = tRect.x + minloc.x;
        rect.y = tRect.y + minloc.y;
    }
    else
    {
        rect.x = rect.y = 0;
        rect.width = rect.height = 0;
    }
}

int main()
{
    // 初始化摄像头，读取视频流
    VideoCapture cap(0);
    // 宽高设置为320x256
    cap.set(CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CAP_PROP_FRAME_HEIGHT, 256);
    // 读取级联分类器
    bool flagGlasses = false;
    if(flagGlasses)
    {
        face_cascade.load("../../imgdata/eyes/haarcascade_frontalface_alt2.xml");
        eye_cascade.load("../../imgdata/eyes/haarcascade_eye.xml");
    }
    else
    {
        face_cascade.load("../../imgdata/eyes/haarcascade_frontalface_alt2.xml");
        eye_cascade.load("../../imgdata/eyes/haarcascade_eye_tree_eyeglasses.xml");
    }
    // 判断初始化设置是否正常
    if(face_cascade.empty() || eye_cascade.empty() || !cap.isOpened()) 
        return -1;
    // 视频流操作
    Mat frame, eyeMat;
    Rect eyeRect;
    while(waitKey(10) != 27)
    {
        cap >> frame;
        if(frame.empty())
            break;
        // 水平反转
        flip(frame, frame, 1);
        // 灰度转换
        Mat gray;
        cvtColor(frame, gray, CV_BGR2GRAY);
        // 判断人眼检测尺寸，若不符合则需要重新检测
        if(eyeRect.width <= 2 || eyeRect.height <= 2)
        {
            // 人眼检测
            detectEye(gray, eyeMat, eyeRect);
        }
        else // 符合则进行人眼跟踪
        {
            // 人眼跟踪
            trackEye(gray, eyeMat, eyeRect);
            // 人眼结果检测
            rectangle(frame, eyeRect, CV_RGB(0,255,0));
        }
        imshow("trackEye", frame);
    }
}
