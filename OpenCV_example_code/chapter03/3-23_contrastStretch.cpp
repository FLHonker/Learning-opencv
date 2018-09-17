// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-23_contrastStretch.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 对比度拉伸，通过扩展图像灰度级动态分布范围实现。
# 对感兴趣区域进行对比度调整，增强画质。
# Core: g(x,y) = f(x,y) * range / 255
# @Create Time: 2018-09-17 14:38:11
# @Last Modified: 2018-09-17 14:38:11
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

/*
 * @input: Mat srcGray, 输入灰度图
 * @return: Mat resultImg 
 */
Mat contrastStretch(Mat srcGray)
{
    Mat resultImg = srcGray.clone();
    int nRows = resultImg.rows, nCols = resultImg.cols;
    // 图像连续性判断
    if(resultImg.isContinuous())
    {
        nCols = nCols * nRows;
        nRows = 1;
    }
    // 图像指针操作
    uchar* pDataMat;
    int pixMin = 255, pixMax = 0;
    // 计算图像的最大值和最小值
    for(int i = 0; i < nRows; ++i)
    {
        pDataMat = resultImg.ptr<uchar>(i);
        for(int j = 0; j < nCols; ++j)
        {
            if(pDataMat[j] > pixMax)
                pixMax = pDataMat[j];
            if(pDataMat[j] < pixMin)
                pixMin = pDataMat[j];
        }
    }
    // 对比度拉伸映射
    for(int  i = 0; i < nRows; ++i)
    {
        pDataMat = resultImg.ptr<uchar>(i);
        for(int j = 0; j < nCols; ++j)
            pDataMat[j] = (pDataMat[j] - pixMin) * 255 / (pixMax - pixMin);
    }
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/panada.jpg");
    if(!srcImg.data)
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    Mat resultImg = contrastStretch(srcGray);
    imshow("resultImage", resultImg);
    waitKey(0);
    return 0;
}
