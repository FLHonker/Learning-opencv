// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-9_detectface_CascadeClassifier.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 级联分类器人脸检测
# @Create Time: 2018-11-22 10:01:41
# @Last Modified: 2018-11-22 10:01:41
***********************************************/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

// 全局变量
CascadeClassifier face_cascade, eyes_cascade;
string window_name = "Face Detection";

// 人脸检测
void detectFaces(Mat frame)
{
    vector<Rect> faces;
    Mat frame_gray;
    // 灰度化
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    // 直方图均衡
    equalizeHist(frame_gray, frame_gray);
    // 多尺度人脸检测
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(30,30));
    // 人脸检测结果判定
    for(size_t i = 0; i < faces.size(); ++i)
    {
        // 检测到人脸中心
        Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        Mat face = frame_gray(faces[i]);
        vector<Rect> eyes;
        // 在人脸区域检测人眼
        eyes_cascade.detectMultiScale(face, eyes, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30,30));
        if(eyes.size() > 0)
        {
            // 绘制人脸
            ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar(255,0,255), 4, 8, 0);
        }
    }
    imshow(window_name, frame);
}

int main()
{
    // 摄像头读取
    VideoCapture cap(0);
    Mat frame;
    // 初始化Haar级联人脸分类器XML
    face_cascade.load("../../imgdata/eyes/haarcascade_frontalface_alt2.xml");
    // 初始化Haar级联人眼分类分类器XML
    eyes_cascade.load("../../imgdata/eyes/haarcascade_eye_tree_eyeglasses.xml");
    while(cap.read(frame))
    {
        // 人脸检测
        detectFaces(frame);
        if(waitKey(30) == 27)
            break;
    }
    return 0;
}
