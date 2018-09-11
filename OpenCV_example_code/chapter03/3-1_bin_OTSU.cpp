// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-1_bin_OTSU.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: OTSU - 寻找图像阈值的最大类间方差算法
# @Create Time: 2018-09-11 09:03:16
# @Last Modified: 2018-09-11 09:03:16
***********************************************/
#include <stdio.h>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// OTSU法函数实现
int OTSU(Mat srcImg)
{
    int nRows = srcImg.rows;
    int nCols = srcImg.cols;
    int threshold = 0;
    // 初始化统计参数
    int nSumPix[256];
    float nProDis[256];
    for(int i = 0; i < 256; ++i)
    {
        nSumPix[i] = 0;
        nProDis[i] = 0;
    }
    // 统计灰度级中每个像素在整幅图中的个数
    for(int i = 0; i < nRows; ++i)
        for(int j = 0; j < nCols; ++j)
            nSumPix[(int)srcImg.at<uchar>(i,j)]++;
    // 计算每个灰度级占图像中的概率分布
    for(int i = 0; i < 256; ++i)
        nProDis[i] = (float)nSumPix[i] / (nRows * nCols);
    // 遍历灰度级[0, 255]，计算出最大类间方差下的阈值
    float w0, w1, u0_temp, u1_temp, u0, u1, delta_temp;
    double delta_max = 0.0;
    for(int i = 0; i < 256; ++i)
    {
        // 初始化相关参数
        w0 = w1 = u0_temp = u1_temp = u0 = u1 = delta_temp = 0;
        for(int j = 0; j < 256; ++j)
        {
            // 背景部分
            if(j <= i)
            {
                // 当前i为分割阈值，第一类总的概率
                w0 += nProDis[j];
                u0_temp += j * nProDis[j];
            }
            // 前景部分
            else
            {
                // 当前i为分割阈值，第二类总的概率
                w1 += nProDis[j];
                u1_temp += j * nProDis[j];
            }
        }
        // 分别计算各类的平均灰度
        u0 = u0_temp / w0;
        u1 = u1_temp / w1;
        delta_temp = (float)(w0 * w1 * pow((u0 - u1), 2));
        // 依次找到最大类间方差下的阈值
        if(delta_temp > delta_max)
        {
            delta_max = delta_temp;
            threshold = i;
        }
    }
    return threshold;
}

int main()
{
    // 图像读取及判断
    Mat srcImg = imread("../../imgdata/baboon.jpg");
    if(!srcImg.data)
        return -1;
    // 灰度化
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    // 调用OTSU二值化法计算阈值
    int otsuThreshold = OTSU(srcGray);
    cout << "otsuThreshold = " << otsuThreshold <<endl;
    // 定义输出结果图像
    Mat otsuResultImg = Mat::zeros(srcGray.rows, srcGray.cols, CV_8UC1);
    // 利用得到的阈值实现二值化操作
    // 其实这里可以直接使用threshold()函数，我们还是自己实现下吧
    for(int i = 0; i < srcGray.rows; ++i)
    {
        for(int j = 0; j < srcGray.cols; ++j)
        {
            // 高像素阈值判断
            if(srcGray.at<uchar>(i,j) > otsuThreshold)
                otsuResultImg.at<uchar>(i,j) = 255;
            else
                otsuResultImg.at<uchar>(i,j) = 0;
        }
    }
    imshow("otsuResultImage", otsuResultImg);
    waitKey(0);
    return 0;
}
