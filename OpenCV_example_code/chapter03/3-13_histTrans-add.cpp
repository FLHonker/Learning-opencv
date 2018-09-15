// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-13_histTrans-add.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 直方图变换——累计
# 步骤：
# (1) 将源图像转换为灰度图，计算图像的直方图；
# (2) 简历映射表， 对直方图进行像素累计；
# (3) 根据映射表进行元素映射得到最终的直方图变换。
# @Create Time: 2018-09-15 15:29:02
# @Last Modified: 2018-09-15 20:17:02
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
    // 计算图像的直方图
    const int channels[1] = { 0 };
    const int histSize[1] = { 256 };
    float hranges[2] = { 0, 255 };
    const float* ranges[1] = { hranges };
    MatND hist;
    calcHist(&srcGray, 1, channels, Mat(), hist, 1, histSize, ranges);
    float table[256];
    int nPix = srcGray.cols * srcGray.rows;
    // 建立映射表
    for(int i = 0; i < 256; ++i)
    {
        float temp = 0;
        // 像素变换
        temp = hist.at<float>(i) / nPix * 255;
        if( i != 0 )
        {
            // 像素累计
            table[i] = table[i - 1] + temp;
        } else {
            table[i] = temp;
        }
    }
    // 通过映射进行表查找
    Mat lookUpTable(Size(1, 256), CV_8U);
    for(int i = 0; i < 256; ++i)
    {
        lookUpTable.at<uchar>(i) = static_cast<uchar>(table[i]);
    }
    Mat histTransResult;
    LUT(srcGray, lookUpTable, histTransResult);
    // 显示图像
    imshow("srcGray", srcGray);
    imshow("histTransResult", histTransResult);
    waitKey(0);
    return 0;
}
