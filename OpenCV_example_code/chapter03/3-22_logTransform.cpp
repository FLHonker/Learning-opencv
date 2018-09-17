// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-22_logTransform.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 对数变换
# @Create Time: 2018-09-17 10:02:33
# @Last Modified: 2018-09-17 10:02:33
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

// 对数变换方法1
Mat logTransform1(Mat srcImg, float c)
{
    // 输入图像判断
    if(srcImg.empty())
        std::cout << "No data!" << std::endl;
    Mat resultImg = Mat::zeros(srcImg.size(), srcImg.type());
    // 计算1+r
    cv::add(srcImg, Scalar(1.0), srcImg);
    // 转换为32位浮点数
    srcImg.convertTo(srcImg, CV_32F);
    // 计算log(1+r)
    cv::log(srcImg, resultImg);
    resultImg = c * resultImg;
    // 归一化处理
    normalize(resultImg, resultImg, 0, 255, NORM_MINMAX);
    convertScaleAbs(resultImg, resultImg);
    return resultImg;
}

// 对数变换方法2
Mat logTransform2(Mat srcImg, float c)
{
    // 输入图像判断
    if(srcImg.empty())
        std::cout << "No data!" << std::endl;
    Mat resultImg = Mat::zeros(srcImg.size(), srcImg.type());
    double gray = 0;
    // 图像遍历，分别计算每个像素点的对数变换
    for(int i = 0; i < srcImg.rows; ++i)
        for(int j = 0; j < srcImg.cols; ++j)
        {
            gray = (double)srcImg.at<uchar>(i,j);
            gray = c * log((double)(1+gray));
            resultImg.at<uchar>(i,j) = saturate_cast<uchar>(gray);
        }
    // 归一化处理
    normalize(resultImg, resultImg, 0, 255, NORM_MINMAX);
    convertScaleAbs(resultImg, resultImg);
    return resultImg;
}

// 对数变换方法3
Mat logTransform3(Mat srcImg, float c)
{
    // 输入图像判断
    if(srcImg.empty())
        std::cout << "No data!" << std::endl;
    Mat resultImg = Mat::zeros(srcImg.size(), srcImg.type());
    // 图像类型转换,32位浮点数
    srcImg.convertTo(resultImg, CV_32F);
    // 图像矩阵元素加1操作
    resultImg = resultImg + 1;
    // 图像对数操作
    cv::log(resultImg, resultImg);
    resultImg = c * resultImg;
    // 图像归一化操作
    normalize(resultImg, resultImg, 0, 255, NORM_MINMAX);
    convertScaleAbs(resultImg, resultImg);
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/apple.jpg");
    if(!srcImg.data)
        return -1;
    // 验证3种不同方式的对数变换速度
    imshow("srcImage", srcImg);
    float c = 1.2;
    Mat resultImg;
    double tTime;   // 时间
    const int nTimes = 100;  // 100次
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
        resultImg = logTransform1(srcImg, c);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    std::cout << "logTrans_1: " << tTime << std::endl;
    imshow("resultImage1", resultImg);
    
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
        resultImg = logTransform2(srcImg, c);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    std::cout << "logTrans_2: " << tTime << std::endl;
    imshow("resultImage2", resultImg);
    
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
        resultImg = logTransform3(srcImg, c);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    std::cout << "logTrans_3: " << tTime << std::endl;
    imshow("resultImage3", resultImg);

    waitKey(0);
    return 0;
}
