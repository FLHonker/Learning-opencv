// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-25_showMBitLayers.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 灰度比特平面 - 数据压缩领域
# 比特平面编码是一种通过单独处理图像位平面来减少像素间冗余的技术，
# 可将一幅多位图像分解成一些列二值图像并进行相应的编码。
# 图像的高阶数据层主要是视觉感知层及轮廓的相关数据，
# 图像低阶数据层主要是对图像细节部分进行刻画。
# @Create Time: 2018-09-17 15:27:12
# @Last Modified: 2018-09-17 15:27:12
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdio>

using namespace cv;

/*
 * @input: Mat srcGray, 输入灰度图 
 */
void showMBitPlan(Mat srcGray)
{
    int nRows = srcGray.rows, nCols = srcGray.cols;
    // 图像连续性判断
    if(srcGray.isContinuous())
    {
        nCols = nCols * nRows;
        nRows = 1;
    }
    // 图像指针操作
    uchar *pSrcMat, *pResultMat;
    Mat resultImg = srcGray.clone();
    int pixMin = 0, pixMax = 0;
    for(int n = 1; n <= 8; ++n)
    {
        // 比特平面分层像素构成
        pixMin = pow(2, n-1);
        pixMax = pow(2, n);
        for(int i = 0; i < nRows; ++i)
        {
            // 获取图像数据指针
            pSrcMat = srcGray.ptr<uchar>(i);
            pResultMat = resultImg.ptr<uchar>(i);
            for(int j = 0; j < nCols; ++j)
            {
                // 相应比特平面层二值化
                if(pSrcMat[j] >= pixMin && pSrcMat[j] < pixMax)
                    pResultMat[j] = 255;
                else
                    pResultMat[j] = 0;
            }
        }
        // 比特平面层输出
        char windowsName[20];
        sprintf(windowsName, "BitPlane_%d", n);
        imshow(windowsName, resultImg);
    }
}

int main()
{
    Mat srcImg = imread("../../imgdata/building2.jpg");
    if(!srcImg.data)
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    showMBitPlan(srcGray);
    waitKey(0);
    return 0;
}
