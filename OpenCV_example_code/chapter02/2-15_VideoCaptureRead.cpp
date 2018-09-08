// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-15_VideoCaptureRead.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 视频文件读取
# @Create Time: 2018-09-08 09:41:46
# @Last Modified: 2018-09-08 09:41:46
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // 定义相关VideoCapture对象
    VideoCapture capture;
    // 读取视频文件
    capture.open("../../imgdata/tree.avi");
    // 判断视频流读取是否正确
    if(!capture.isOpened())
    {
        std::cout << "fail to open video!" << std::endl;
        return -1;
    }
    // 获取视频相关信息——帧数
    long nTotalFrame = capture.get(CV_CAP_PROP_FRAME_COUNT);
    cout << "nTotalFrame = " << nTotalFrame << endl;
    // 获取视频相关信息——帧像素宽、高
    int frameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    int frameWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    cout << "frameHeight = " << frameHeight << endl;
    cout << "frameWidth = " << frameWidth << endl; 
    // 获取视频相关信息——帧率
    double frameRate = capture.get(CV_CAP_PROP_FPS);
    cout << "frameRate = " << frameRate << endl;

    Mat frameImg;
    // read方法获取显示帧
    long nCount = 1;
    while(true)
    {
        // 输出当前帧号
        cout << "current frame: " << nCount << endl;
        capture >> frameImg;
        // 判断当前读取文件
        if(!frameImg.empty())
            imshow("frameImg", frameImg);
        else
            break;
        // 按下键盘上的Q键退出
        if(char(waitKey(10)) == 'q')
            break;
        nCount++;
    }
    // 视频释放
    capture.release();
    return 0;
}
