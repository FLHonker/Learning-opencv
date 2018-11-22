// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-11_TLD.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 单目标跟踪
# @Create Time: 2018-11-22 12:09:19
# @Last Modified: 2018-11-22 12:09:19
***********************************************/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <vector>

using namespace cv;

// 移动鼠标 选取矩形框
void mouseClickCallback(int event,
                        int x, int y, int flags, void* userdata)
{
    // 矩形数据返回
    cv::Rect2d * pRect =
        reinterpret_cast<cv::Rect2d*>(userdata);

    // 鼠标按下操作
    if (event == cv::EVENT_LBUTTONDOWN) {
        std::cout << "LBUTTONDOWN ("
                  << x << ", " << y << ")" << std::endl;
        // 获取x，y坐标
        pRect->x = x;
        pRect->y = y;
    }
    // 鼠标抬起操作
    else if (event == cv::EVENT_LBUTTONUP) {
        std::cout << "LBUTTONUP ("
                  << x << ", " << y << ")" << std::endl;
        // 获取矩形宽高
        pRect->width =  std::abs(x - pRect->x);
        pRect->height = std::abs(y - pRect->y);
    }
}
int main(int argc, char** argv)
{
    // 读取视频流
    VideoCapture cap("../../imgdata/car.avi");

    if (!cap.isOpened()) {
        std::cout << " on data! " << std::endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_POS_MSEC, 2 * 1000);
    Mat frame;
    // 初始化TLD追踪器
    Rect2d *rect(new cv::Rect2d);
    Ptr<TrackerTLD> tracker = TrackerTLD::create();
    // 读取第一帧初始化矩形框
    cap >> frame;
    resize(frame, frame, Size(), 0.25, 0.25);
    imshow("TrackerTLD", frame);
    // 鼠标移动获取矩形区域
    cv::setMouseCallback("TrackerTLD", mouseClickCallback,
                         reinterpret_cast<void*>(rect));
    waitKey(0);

    if (rect->area() == 0.0)
        return -1;

    // 跟踪器初始
    tracker->init(frame, *rect);
    double fps = 1.0;

    while (true) 
    {
        cap >> frame;
        resize(frame, frame, Size(), 0.25, 0.25);

        if (frame.empty())
            break;

        // 追踪器更新
        if (tracker->update(frame, *rect))
            // 绘制追踪结果
            cv::rectangle(frame, *rect, Scalar(255, 0, 0), 2, 1);

        imshow("TrackerTLD", frame);

        if (27 == waitKey(10))
            break;
    }

    cap.release();
    return 0;
}
