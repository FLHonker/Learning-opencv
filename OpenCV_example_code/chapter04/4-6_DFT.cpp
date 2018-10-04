// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-6_DFT.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 离散傅里叶变换
# @Create Time: 2018-10-02 19:58:54
# @Last Modified: 2018-10-02 19:58:54
***********************************************/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat DFT(Mat srcImg)
{
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 将图像延扩到最佳尺寸
    int nRows = getOptimalDFTSize(srcGray.rows);
    int nCols = getOptimalDFTSize(srcGray.cols);
    Mat resultImg;
    // 把灰度图像放在左上角，向右和向下扩展图像
    // 将添加的像素初始化为0
    copyMakeBorder(srcGray, resultImg, 0, nRows - srcGray.rows, 0, nCols - srcGray.cols, BORDER_CONSTANT, Scalar::all(0));
    // 为傅里叶变换的结果(实部和虚部)分配存储空间
    Mat planes[] = { Mat_<float>(resultImg), Mat::zeros(resultImg.size(), CV_32F) };
    Mat completeI;
    // 为延扩后的图像增添一个初始化为0的通道
    merge(planes, 2, completeI);
    // 进行离散傅里叶变换
    dft(completeI, completeI);
    // 将复数转换为幅度
    split(completeI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat dftResultImg = planes[0];
    // 对数尺度(logarithm scale)缩放
    dftResultImg += 1;
    log(dftResultImg, dftResultImg);
    // 剪切和重分布幅度图象限
    dftResultImg = dftResultImg(Rect(0,0,srcGray.cols,srcGray.rows));
    // 归一化图像
    normalize(dftResultImg, dftResultImg, 0, 1, CV_MINMAX);
    int cx = dftResultImg.cols / 2;
    int cy = dftResultImg.rows / 2;
    Mat tmp;
    // Top-Left——为每一个象限创建ROI
    Mat q0(dftResultImg, Rect(0, 0, cx, cy));
    // Top-Right
    Mat q1(dftResultImg, Rect(cx, 0, cx, cy));
    // Bottom-Left
    Mat q2(dftResultImg, Rect(0, cy, cx, cy));
    // Bottom-Right
    Mat q3(dftResultImg, Rect(cx, cy, cx, cy));
    // 交换象限(Top-Left <--> Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    // 交换象限(Top-Right <--> Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    return dftResultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/butterfly.jpg");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    Mat resultImg = DFT(srcImg);
    imshow("dftImage", resultImg);
    waitKey(0);
    return 0;
}
