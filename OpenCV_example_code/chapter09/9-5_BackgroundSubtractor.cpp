// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-5_BackgroundSubtractor.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 混合高斯背景建模
# @Create Time: 2018-11-20 10:36:22
# @Last Modified: 2018-11-20 10:36:22
***********************************************/
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

// 定义全局变量
Mat frame,  fgMaskMog2;
Ptr<BackgroundSubtractor> pMOG2;
int keyboard;

void processVideo(string videoFilename)
{
    // 视频捕获
    VideoCapture capture(videoFilename);
    if(!capture.isOpened())
    {
        // 输出错误信息
        cerr << "Unaable to open video file:" << videoFilename  <<endl;
        exit(EXIT_FAILURE);
    }
    // 按下Q键或ESC退出
    while((char)keyboard != 'q' && (char)keyboard != 27)
    {
        // 读取当前帧
        if(!capture.read(frame))
        {
            cerr << "Unable to read next frame." <<endl;
            exit(EXIT_FAILURE);
        }
        // 缩小图像尺寸
        resize(frame, frame, Size(), 0.25, 0.25);
        // 生成背景模型
        pMOG2->apply(frame, fgMaskMog2);
        // 输出当前帧号
        stringstream ss;
        rectangle(frame, Point(10,2), Point(100,20), Scalar(255,255,255), -1);
        // 得到当前视频图像的帧号
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumStr = ss.str();
        // 左上角显示帧号
        putText(frame, frameNumStr.c_str(), Point(15,15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
        // 输出结果
        imshow("Frame", frame);
        imshow("FG Mask MOG2", fgMaskMog2);
        keyboard = waitKey(30);
    }
    capture.release();
}

int main(int argc, char** argv)
{
    // 创建背景模型类
    pMOG2 = createBackgroundSubtractorMOG2();
    string inputPath = "../../imgdata/car.avi";
    processVideo(inputPath);
    return 0;
}
