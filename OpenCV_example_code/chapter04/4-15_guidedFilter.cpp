// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-15_guidedFilter.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 导向滤波
# @Create Time: 2018-10-06 11:01:21
# @Last Modified: 2018-10-06 11:01:21
***********************************************/
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// 导向滤波器
Mat guidedFilter(Mat &src, Mat &src2, int r, double eps)
{
    // 转换源图像信息
    src.convertTo(src, CV_64FC1);
    src2.convertTo(src2, CV_64FC1);
    int nRows = src.rows, nCols = src.cols;
    Mat boxResult;
    // 步骤一：计算均值
    boxFilter(Mat::ones(nRows, nCols, src.type()), boxResult, CV_64FC1, Size(r,r));
    // 生成导向均值mean_I
    Mat mean_I;
    boxFilter(src, mean_I, CV_64FC1, Size(r,r));
    // 生成原始均值mean_p
    Mat mean_p;
    boxFilter(src2, mean_p, CV_64FC1, Size(r,r));
    // 生成互相关均值mean_Ip
    Mat mean_Ip;
    boxFilter(src.mul(src2), mean_Ip, CV_64FC1, Size(r,r));
    Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);
    // 生成自相关均值mean_II
    Mat mean_II;
    // 应用盒滤波器计算相关均值
    boxFilter(src.mul(src), mean_II, CV_64FC1, Size(r,r));

    // 步骤二：计算相关系数
    Mat var_I = mean_II - mean_I.mul(mean_I);
    Mat var_Ip = mean_Ip - mean_I.mul(mean_p);

    // 步骤三：计算参数系数a、b
    Mat a = cov_Ip / (var_I + eps);
    Mat b = mean_p - a.mul(mean_I);

    // 步骤四：计算系数a、b的均值
    Mat mean_a, mean_b;
    boxFilter(a, mean_a, CV_64FC1, Size(r,r));
    mean_a = mean_a / boxResult;
    boxFilter(b, mean_b, CV_64FC1, Size(r,r));
    mean_b = mean_b / boxResult;
    
    // 步骤五：生成输出矩阵
    Mat resultMat = mean_a.mul(src) + mean_b;

    return resultMat;
}

int main()
{
    Mat srcImg = imread("../../imgdata/butterfly.jpg");
    if(srcImg.empty())
        return -1;
    // 通道分离
    vector<Mat> vSrcImg, vResultImg;
    split(srcImg, vSrcImg);
    Mat resultImg;
    for(int i = 0; i < 3; ++i)
    {
        // 分通道转换成浮点数据类型
        Mat tempImg;
        vSrcImg[i].convertTo(tempImg, CV_64FC1, 1.0 / 255.0);
        Mat p = tempImg.clone();
        // 分别进行导向滤波
        Mat result = guidedFilter(tempImg, p, 4, 0.01);
        vResultImg.push_back(result);
    }
    // 通道结果合并
    merge(vResultImg, resultImg);
    imshow("srcImage", srcImg);
    imshow("guidedFilterImage", resultImg);
    waitKey(0);
    return 0;
}
