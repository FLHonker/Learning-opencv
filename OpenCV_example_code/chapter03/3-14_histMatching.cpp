// C9999999999999999999999999999999999999999999999999998/++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-14_histMatching.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 直方图匹配（直方图规格化）
# 步骤：
# (1) 分别计算源图像和目标图像的累计概率分布；
# (2) 分别对源图像和目标图像进行直方图均衡化操作；
# (3) 利用组映射关系使源图像直方图按照规定进行变换。
# @Create Time: 2018-09-15 20:37:31
# @Last Modified: 2018-09-15 20:37:31
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // 加载图像并显示
    Mat srcImg = imread("../../imgdata/fruits.jpg");
    Mat dstImg = imread("../../imgdata/blox.jpg");
    if(!srcImg.data || !dstImg.data)
        return -1;
    resize(dstImg, dstImg, Size(srcImg.rows, srcImg.cols), 0, 0, CV_INTER_LINEAR);
    imshow("srcImage", srcImg);
    imshow("dstImage", dstImg);
    waitKey(0);

    // 初始化累计分布参数
    float srcCdArr[256], dstCdArr[256];
    int srcAddTemp[256], dstAddTemp[256], histMatchMap[256];
    for(int i = 0; i < 256; ++i)
    {
        srcAddTemp[i] = 0;
        dstAddTemp[i] = 0;
        srcCdArr[i] = 0;
        dstCdArr[i] = 0;
        histMatchMap[i] = 0;
    }
    float sumSrcTemp = 0, sumDstTemp = 0;
    int nSrcPix = srcImg.rows * srcImg.cols;
    int nDstPix = dstImg.rows * dstImg.cols;
    int matchFlag = 0;
    // 求解源图像与目标图像的累计直方图
    for(size_t nrow = 0; nrow < srcImg.rows; ++nrow)
    {
        for(size_t ncol = 0; ncol < srcImg.cols; ++ncol)
        {
            srcAddTemp[(int)srcImg.at<uchar>(nrow, ncol)]++;
            dstAddTemp[(int)dstImg.at<uchar>(nrow, ncol)]++;
        }
    }
    // 求解源图像与目标图像的累计概率分布
    for(int i = 0; i < 256; ++i)
    {
        sumSrcTemp += srcAddTemp[i];
        srcCdArr[i] = sumSrcTemp / nSrcPix;
        sumDstTemp += dstAddTemp[i];
        dstCdArr[i] = sumDstTemp / nDstPix;
    }
    // 直方图匹配
    for(int i = 0; i < 256; ++i) 
    {
        float minMatchPara = 20;
        for(int j = 0; j < 256; ++j)
        {
            // 判断当前直方图累计差异
            if(minMatchPara > abs(srcCdArr[i] - dstCdArr[j]))
            {
                minMatchPara = abs(srcCdArr[i] - dstCdArr[j]);
                matchFlag = j;
            }
        }
        histMatchMap[i] = matchFlag;                              
    }
    // 初始化匹配图像
    Mat histMatchImg = Mat::zeros(srcImg.rows, srcImg.cols, CV_8UC3);
    cvtColor(srcImg, histMatchImg, CV_BGR2GRAY);
    // 通过map映射成匹配图像
    for(int i = 0; i < histMatchImg.rows; ++i)
        for(int j = 0; j < histMatchImg.cols; ++j)
            histMatchImg.at<uchar>(i, j) = histMatchMap[(int)histMatchImg.at<uchar>(i, j)];
    
    imshow("histMatchedImage", histMatchImg);
    waitKey(0);
    return 0;
}
