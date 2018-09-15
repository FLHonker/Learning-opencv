// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-12_histTrans-find.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 直方图变换——查找
# 直方图变化查找方法实现思路:
# (1) 将源图像转换为灰度图，计算图像的灰度直方图；
# (2) 根据预设阈值参数由低到高查找iLow，在由高到低查找iHigh；
# (3) 根据上步得到直方图iLow与iHigh并进行查找表变换；
# (4) 通过查找表进行映射变换， 完成直方图查找方法变换。
# @Create Time: 2018-09-15 15:29:02
# @Last Modified: 2018-09-15 15:29:02
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/butterfly.jpg");
    if(!srcImg.data)
        return -1;
    // 转化为灰度图
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 第一步：计算图像的直方图
    const int channels[1] = { 0 };
    const int histSize[1] = { 256 };
    float hranges[2] = { 0, 255 };
    const float* ranges[1] = { hranges };
    MatND hist;
    calcHist(&srcGray, 1, channels, Mat(), hist, 1, histSize, ranges);

    // 第二步：根据预设参数统计灰度级变换
    int segThreshold = 50;
    // 由低到高进行查找
    int iLow = 0;
    for(; iLow < histSize[0]; ++iLow)
    {
        if(hist.at<float>(iLow) > segThreshold)
            break;
    }
    // 由高到低进行查找
    int iHigh = histSize[0] - 1;
    for(; iHigh >= 0; --iHigh)
    {
        if(hist.at<float>(iHigh) > segThreshold)
            break;
    }

    // 第三步：建立查找表
    Mat lookUpTable(Size(1, 256), CV_8U);
    for(int i = 0; i < 256; ++i)
    {
        if(i < iLow)
            lookUpTable.at<uchar>(i) = 0;
        else if(i > iHigh)
            lookUpTable.at<uchar>(i) = 255;
        else
            lookUpTable.at<uchar>(i) = static_cast<uchar>(255.0 * (i - iLow) / (iHigh - iLow + 0.5 ));
    }
    
    // 第四步：通过查找表进行映射变换
    Mat histTransResult;
    LUT(srcGray, lookUpTable, histTransResult);
    // 显示图像
    imshow("srcGray", srcGray);
    imshow("histTransResult", histTransResult);
    waitKey(0);
    return 0;
}
