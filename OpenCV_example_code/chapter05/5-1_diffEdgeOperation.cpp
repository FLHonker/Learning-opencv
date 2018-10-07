// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-1_diffEdgeOperation.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 差分边缘检测
# @Create Time: 2018-10-07 08:46:42
# @Last Modified: 2018-10-07 08:46:42
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// 图像差分操作
void diffOperation(const Mat srcImg, Mat& edgeXImg, Mat& edgeYImg)
{
    Mat tempImg = srcImg.clone();
    int nRows = tempImg.rows, nCols = tempImg.cols;
    for(int i = 0; i < nRows - 1; ++i)
    {
        for(int j = 0; j < nCols - 1; ++j)
        {
            // 计算垂直边缘
            edgeXImg.at<uchar>(i, j) = abs(tempImg.at<uchar>(i+1, j) - 
                                           tempImg.at<uchar>(i, j));
            // 计算水平边缘
            edgeYImg.at<uchar>(i, j) = abs(tempImg.at<uchar>(i, j+1) - 
                                           tempImg.at<uchar>(i, j));
        }
    }
}

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    Mat edgeXImg(srcImg.size(), srcImg.type());
    Mat edgeYImg(srcImg.size(), srcImg.type());
    // 计算差分图像
    diffOperation(srcImg, edgeXImg, edgeYImg);
    imshow("edgeXImage", edgeXImg);
    imshow("edgeYImage", edgeYImg);
    Mat edgeImg(srcImg.size(), srcImg.type());
    // 水平与垂直边缘图像叠加
    addWeighted(edgeXImg, 0.5, edgeYImg, 0.5, 0.0, edgeImg);
    imshow("edgeImage", edgeImg);
    waitKey(0);
    return 0;
}
