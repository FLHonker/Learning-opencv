// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-16_VideoWriter.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 视频的写入
# @Create Time: 2018-09-08 10:11:05
# @Last Modified: 2018-09-08 10:11:05
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // 设置视频读入与输出路径
    string sourceVideoPath = "../../imgdata/tree.avi";
    string outputVideoPath = "../out/videowriter.mp4";
    // 视频输入
    VideoCapture inputVideo(sourceVideoPath);
    if(!inputVideo.isOpened())
    {
        cout << "fail to open video!" <<endl;
        return -1;
    }
    VideoWriter outputVideo;
    // 获取视频分辨率
    Size videoResolution = Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
                                (int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    // 获取视频帧总数
    cout << "totalFrame = " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    // 获取视频帧率
    double fps = inputVideo.get(CV_CAP_PROP_FPS);
    cout << "fps = " << fps <<endl;
    // 获取视频图像宽高
    cout << "videoResolution: " << videoResolution.width << " x " << videoResolution.height << endl;
    // open方法相关设置
    outputVideo.open(outputVideoPath, CV_FOURCC('X','V','I','D'), 25.0, videoResolution, true);
    if(!outputVideo.isOpened())
    {
        cout << "fail to open outputVideo!" <<endl;
        return -1;
    }
    Mat frameImg;
    int count = 0;
    // vector RGB分量
    std::vector<Mat> rgb;
    Mat resultImg;
    while(true)
    {
        inputVideo >> frameImg;
        // 视频帧结束判断
        if(frameImg.empty())
            break;
        count++;
        imshow("framImg", frameImg);
        // 分离出3通道rgb
        cv::split(frameImg, rgb);
        for(int i = 0; i < 3; ++i)
        {
            if(i != 0)
            {
                // 提取B通道分量
                rgb[i] = Mat::zeros(videoResolution, rgb[0].type());
            }
            // 通道合并
            cv::merge(rgb, resultImg);
        }
        imshow("resultImg", resultImg);
        outputVideo << resultImg;

        // 按下键盘上Q键退出
        if(char(waitKey(0)) == 'q')
            break;
    }
    cout << "writerTotalFrame: " << count <<endl;
    return 0;
}
