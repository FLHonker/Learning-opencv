// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-1_scaleSpace.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 特征尺度变换
# @Create Time: 2018-10-21 20:16:25
# @Last Modified: 2018-10-21 20:16:25
***********************************************/
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

void CreateScaleSpace(Mat src, vector<vector<Mat>> &scaleSpace, vector<vector<Mat>> &DoG)
{
    Size ksize(5, 5);
    Mat srcBlurMat, up, down;
    // 高斯平滑
    GaussianBlur(src, srcBlurMat, ksize, 0.5);
    // 金字塔
    pyrUp(srcBlurMat, up);
    up.copyTo(scaleSpace[0][0]);
    // 高斯平滑
    GaussianBlur(scaleSpace[0][0], scaleSpace[0][0], ksize, 1.0);
    // 图像遍历
    for(int i = 0; i < 4; ++i)
    {
        // 平滑因子
        double sigma = 1.4142135;
        for(int j = 0; j < 5+2; ++j)
        {
            sigma = sigma * pow(2.0, j/2.0);
            // 对下一尺度进行高斯操作
            GaussianBlur(scaleSpace[i][j], scaleSpace[i][j+1], ksize, sigma);
            // 生成多尺度空间
            DoG[i][j] = scaleSpace[i][j] - scaleSpace[i][j+1];
            // 输出对应特征空间尺度
            cout << "iave:" << i << ", Scale:" << j << ", size:" << 
                scaleSpace[i][j].rows << "x" << scaleSpace[i][j].cols <<endl;
        }
        // 如果不能完成，继续进行金字塔下采样
        if(i < 3)
        {
            // 金字塔下采样
            pyrDown(scaleSpace[i][0], down);
            down.copyTo(scaleSpace[i+1][0]);
        }
    }
}
