// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-7_BackgroundSubtractorGMG.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: KNN模型背景建模
# @Create Time: 2018-11-21 11:05:10
# @Last Modified: 2018-11-21 11:05:10
***********************************************/
#include <opencv2/bgsegm.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;
using namespace bgsegm;

// GMG目标建模检测
void detectBackground(Ptr<BackgroundSubtractorGMG> pBackgroundGMG, string videoFilename)
{
    Mat frame, fgMask;
    int keyboard = 0;
    // 视频获取
    VideoCapture capture(videoFilename);
    if(!capture.isOpened())
        exit(EXIT_FAILURE);
    // 按下Q键或ESC退出
    while((char)keyboard != 'q' && (char)keyboard != 27)
    {
        // 读取当前帧
        if(!capture.read(frame))
            exit(EXIT_FAILURE);
        // 缩小图像尺寸
        resize(frame, frame, Size(), 0.2, 0.2);
        // 设定背景建模参数
        pBackgroundGMG->setDefaultLearningRate(0.05);
        pBackgroundGMG->setDecisionThreshold(0.9);
        pBackgroundGMG->setSmoothingRadius(7);
        pBackgroundGMG->setNumFrames(100);
        // 生成背景模型
        pBackgroundGMG->apply(frame, fgMask);
        // 输出当前帧号
        stringstream ss;
        // 设定矩形区域
        rectangle(frame, Point(10, 2), Point(100, 20), Scalar(255,255,255), -1);
        // 获取当前视频的帧信息
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumStr = ss.str();
        // 左上角显示帧号
        putText(frame, frameNumStr.c_str(), Point(15,15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
        // 输出结果
        imshow("frame", frame);
        imshow("fgMask", fgMask);
        keyboard = waitKey(30);
    }
    // 视频流释放
    capture.release();
}

int main(int argc, char** argv)
{
    // 创建GMG背景建模类
    Ptr<BackgroundSubtractorGMG> pBackgroundGMG = createBackgroundSubtractorGMG();
    string inputPath = "../../imgdata/car.avi";
    // 背景建模检测
    detectBackground(pBackgroundGMG, inputPath);
    return 0;
}
