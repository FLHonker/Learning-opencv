// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-25_moravecCorners.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: moravec角点检测
# @Create Time: 2018-10-13 11:12:44
# @Last Modified: 2018-10-13 11:12:44
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;

// MoravecCorners角点检测
Mat MoravecCorners(Mat src, int kSize, int threshold)
{
    Mat resMorMat = src.clone();
    // 获取初始化参数信息
    int r = kSize / 2;
    const int nRows = src.rows;
    const int nCols = src.cols;
    int nCount = 0;
    CvPoint *pPoint = new CvPoint[nRows * nCols];
    // 图像遍历
    for(int i = r; i < src.rows - r; ++i)
    {
        for(int j = r; j < src.cols - r; ++j)
        {
            int wV1, wV2, wV3, wV4;
            wV1 = wV2 = wV3 = wV4 = 0;
            // 计算水平方向(0°)窗内的兴趣值
            for(int k = -r; k < r; ++k)
                wV1 += (src.at<uchar>(i, j+k) - src.at<uchar>(i, j+k+1)) *
                    (src.at<uchar>(i, j+k) - src.at<uchar>(i, j+k+1));
            // 计算垂直方向(90°)窗内的兴趣值
            for(int k = -r; k < r; ++k)
                wV2 += (src.at<uchar>(i+k, j) - src.at<uchar>(i+k+1, j)) *
                    (src.at<uchar>(i+k, j) - src.at<uchar>(i+k+1, j));
            // 计算45°方向窗内的兴趣值
            for(int k = -r; k < r; ++k)
                wV3 += (src.at<uchar>(i+k, j+k) - src.at<uchar>(i+k+1, j+k+1)) *
                    (src.at<uchar>(i+k, j+k) - src.at<uchar>(i+k+1, j+k+1));
            // 计算135°方向窗内的兴趣值
            for(int k = -r; k < r; ++k)
                wV4 += (src.at<uchar>(i+k, j-1) - src.at<uchar>(i+k+1, j-k-1)) *
                    (src.at<uchar>(i+k, j-k) - src.at<uchar>(i+k+1, j-k-1));
            // cout<< wV1 << "," << wV2 << "," << wV3 << "," << wV4 <<endl;
            // 取其中的最小值作为该像素点的最终兴趣值
            int value = min(min(wV1, wV2), min(wV3, wV4));
            // 若兴趣值大于阈值，则将点的坐标存入数组中
            if(value > threshold)
            {
                pPoint[nCount++] = CvPoint(j, i);
            }
        }
    }
    // 绘制兴趣点
    for(int i = 0; i < nCount; ++i)
        circle(resMorMat, pPoint[i], 4, Scalar(255,0,0));

    return resMorMat;
}

int main()
{
    Mat srcImg= imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
        return -1;
    Mat resMorMat = MoravecCorners(srcImg, 5, 10000);
    imshow("srcImage", srcImg);
    imshow("resMorMat", resMorMat);
    waitKey(0);
    return 0;
}
