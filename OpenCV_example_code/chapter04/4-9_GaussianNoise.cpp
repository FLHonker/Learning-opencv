// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-9_GaussianNoise.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 高斯噪声
# @Create Time: 2018-10-04 20:45:27
# @Last Modified: 2018-10-04 20:45:27
***********************************************/
#include <cstdlib>
#include <cmath>
#include <limits>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

double genGaussianNoise(double mu, double sigma)
{
    // 定义最小值
    const double epsilon = std::numeric_limits<double>::min();
    static double z0, z1;
    static bool flag = false;
    flag = !flag;
    // flag为假构造高斯随机变量X
    if(!flag)
        return z1 * sigma + mu;
    double u1, u2;
    // 构造随机变量
    do {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    }while(u1 <= epsilon);
    // flag为真构造高斯随机变量X
    z0 = sqrt(-2.0 * log(u1)) * cos(2 * CV_PI * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(2 * CV_PI * u2);
    return z0 * sigma + mu;
}

// 为图像添加高斯噪声
Mat addGaussianNoise(Mat& srcImg)
{
    Mat resultImg = srcImg.clone();
    int channels = resultImg.channels();
    int nRows = resultImg.rows;
    int nCols = resultImg.cols * channels;
    // 判断图像的连续性
    if(resultImg.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }
    for(int i = 0; i < nRows; ++i)
    {
        for(int j = 0; j < nCols; ++j)
        {
            // 添加高斯噪声
            int val = resultImg.ptr<uchar>(i)[j] + genGaussianNoise(2, 0.8) * 32;
            if(val < 0 )
                val = 0;
            else if(val > 255)
                val = 255;
            resultImg.ptr<uchar>(i)[j] = (uchar)val;
        }
    }
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/forest.jpg", 1);
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    Mat resultImg = addGaussianNoise(srcImg);
    imshow("gaussianImage", resultImg);
    waitKey(0);
    return 0;
}
