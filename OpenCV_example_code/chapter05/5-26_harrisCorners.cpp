// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-26_harrisCorners.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: harris角点检测
# @Create Time: 2018-10-13 16:33:08
# @Last Modified: 2018-10-13 16:33:08
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// 计算harris角点
void HarrisCorners(const Mat& src, Mat& dst, int blockSize, int kSize, double k)
{
    dst.create(src.size(), CV_32F);
    int depth = src.depth();
    // 检测掩膜尺寸
    double scale = (double)(1 << ((kSize > 0 ? kSize : 3) - 1)) * blockSize;
    if(depth == CV_8U)
        scale *= 255.0;
    scale = 1. / scale;
    // Sobel滤波
    Mat dx, dy;
    Sobel(src, dx, CV_32F, 1, 0, kSize, scale, 0);
    Sobel(src, dy, CV_32F, 0, 1, kSize, scale, 0);
    Size size = src.size();
    Mat cov(size, CV_32FC3);
    int i, j;
    for(i = 0; i < size.height; ++i)
    {
        float* covData = (float*)(cov.data + i*cov.step);
        const float* dxData = (const float*)(dx.data + i*dx.step);
        const float* dyData = (const float*)(dy.data + i*dy.step);
        for(j = 0; j < size.width; ++j)
        {
            float dx_ = dxData[j];
            float dy_ = dyData[j];
            covData[3 * j] = dx_ * dx_;
            covData[3 * j +1] = dx_ * dy_;
            covData[3 * j +2] = dy_ * dy_;
        }
    }
    // 对图像进行盒滤波
    boxFilter(cov, cov, cov.depth(), Size(blockSize, blockSize), Point(-1, -1), false);
    // 判断图像连续性
    if(cov.isContinuous() && dst.isContinuous())
    {
        size.width = size.height * size.width;
        size.height = 1;
    } else {
        size = dst.size();
    }
    // 计算响应函数
    for(i = 0; i < size.height; ++i)
    {
        // 获取图像矩阵指针
        float* dstData = (float*)(dst.data + i*dst.step);
        const float* covData = (const float*)(cov.data + i*cov.step);
        for(j = 0; j < size.width; ++j)
        {
            // 角点响应生成
            float a = covData[3 * j];
            float b = covData[3 * j +1];
            float c = covData[3 * j+2];
            dstData[j] = a*c - b*b -k*(a+c)*(a+c);
        }
    }
}

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    Mat srcGray, dstImg;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    dstImg = Mat::zeros(srcImg.size(), CV_32FC1);
    // 角点检测参数
    int blockSize = 2;
    int apertureSize = 3;

    double k = 0.04;
    // 角点检测
    //cornerHarris(srcGray, dstImg, blockSize, apertureSize, k, BORDER_DEFAULT);
    HarrisCorners(srcGray, dstImg, blockSize, apertureSize, k);
    // 矩阵归一化
    normalize(dstImg, dstImg, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dstImg, dstImg);
    // 绘制角点检测结果
    for(int i = 0; i < dstImg.rows; ++i)
    {
        for(int j = 0; j < dstImg.cols; ++j)
        {
            if((int)dstImg.at<float>(i,j) > 150)
                circle(srcImg, Point(j, i), 5, Scalar(0), 2, 8, 0);
        }
    }
    imshow("cov", dstImg);
    imshow("harris", srcImg);
    waitKey(0);
    return 0;
}
