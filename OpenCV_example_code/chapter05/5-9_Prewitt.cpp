// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-9_Prewitt.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 基本边缘检测算子——Prewitt
# @Create Time: 2018-10-09 10:10:24
# @Last Modified: 2018-10-09 10:10:24
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// Prewitt算子实现
Mat prewitts(Mat src, bool verFlag = false)
{
    src.convertTo(src, CV_32FC1);
    Mat prewitt_kernel = (Mat_<float>(3,3) << 0.1667, 0.1667, 0.1667,
                                              0, 0, 0,
                                              -0.1667, -0.1667, -0.1667);
    // 垂直边缘
    if(verFlag)
    {
        prewitt_kernel = prewitt_kernel.t();
        Mat z1 = Mat::zeros(src.rows, 1, CV_32FC1);
        Mat z2 = Mat::zeros(1, src.cols, CV_32FC1);
        // 将图像的四边设为0
        z1.copyTo(src.col(0));
        z1.copyTo(src.col(src.cols - 1));
        z2.copyTo(src.row(0));
        z2.copyTo(src.row(src.rows - 1));
    }
    Mat edges;
    filter2D(src, edges, src.type(), prewitt_kernel);
    Mat mag;
    multiply(edges, edges, mag);
    // 去除垂直边缘边界黑边
    if(verFlag)
    {
        Mat black_region = src < 0.3;   // 比较元素生成0-1矩阵
        Mat se = Mat::ones(5, 5, CV_8UC1);
        dilate(black_region, black_region, se);
        mag.setTo(0, black_region);
    }
    // 根据模长计算出梯度的阈值
    double thresh = 4.0f * mean(mag).val[0];
    // 仅在某点梯度大于水平方向或垂直方向的邻点梯度时，
    // 才设改位置的输出值为255，并应用阈值thresh
    Mat dstImg = Mat::zeros(mag.size(), mag.type());
    float* dptr = (float*)mag.data;
    float* tptr = (float*)dstImg.data;
    int r = edges.rows, c = edges.cols;
    for(int i = 1; i < r-1; ++i)
    {
        for(int j = 1; j < c-1; ++j)
        {
            // 非极大值抑制
            bool b1 = dptr[i*c +j] > dptr[i*c +j -1];
            bool b2 = dptr[i*c +j] > dptr[i*c +j +1];
            bool b3 = dptr[i*c +j] > dptr[(i-1)*c +j];
            bool b4 = dptr[i*c +j] > dptr[(i+1)*c +j];
            tptr[i*c +j] = 255 * ((dptr[i*c +j] > thresh) && 
                                  (b1 && b2 || b3 && b4));
        }
    }
    dstImg.convertTo(dstImg, CV_8UC1);
    return dstImg;
}

// test
int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
        return -1;
    Mat dstImg = prewitts(srcImg);
    imshow("srcImage", srcImg);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
