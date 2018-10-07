// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-2-5_Sobel.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Sobel边缘检测算子
# @Create Time: 2018-10-07 13:01:52
# @Last Modified: 2018-10-07 13:01:52
***********************************************/
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

/*****××**** 1.非极大值抑制实现Sobel数值细化边缘 ************/

bool SobelVerEdge(Mat src, Mat& dst)
{
    CV_ASSERT(src.channels() == 1);
    src.convertTo(src, CV_32FC1);
    // 水平方向的Sobel算子
    Mat sobelX = (Mat_<float>(3,3) << -0.125, 0, 0.125,
                                       -0.25, 0, 0.25,
                                      -0.125, 0, 0.125);
    Mat ConResMat;   // 卷积运算结果
    // 卷积运算
    filter2D(src, ConResMat, src.type(), sobelX);
    // 计算梯度的幅度
    Mat graMagMat;
    multiply(ConResMat, ConResMat, graMagMat);
    // 根据梯度幅度及参数设置阈值
    int scaleVal = 4;
    double thresh = scaleVal * mean(graMagMat).val[0];
    Mat dstTempMat = Mat::zeros(graMagMat.size(), graMagMat.type());
    float* pDataMag = (float*)graMagMat.data;
    float* pDataDst = (float*)dstTempMat.data;
    const int nRows = ConResMat.rows;
    const int nCols = ConResMat.cols;
    for(int i = 1; i != nRows - 1; ++i)
    {
        for(int j = 1; j != nCols - 1; ++j)
        {
            // 计算改点梯度与水平或垂直梯度值的大小并比较结果
            bool b1 = pDataMag[i * nCols + j] > pDataMag[i * nCols +j -1];
            bool b2 = pDataMag[i * nCols + j] > pDataMag[i * nCols +j +1];
            bool b3 = pDataMag[i * nCols + j] > pDataMag[(i - 1) * nCols + j];
            bool b4 = pDataMag[i * nCols + j] > pDataMag[(i + 1) * nCols + j];
            // 判断领域梯度是否满足大于水平或垂直梯度的条件，
            // 并根据自适应阈值参数进行二值化
            pDataDst[i * nCols + j] = 255 * (pDataMag[i * nCols + j] > thresh && (b1 && b2 || b3 && b4));
        }
    }
    dstTempMat.convertTo(dstTempMat, CV_8UC1);
    dst = dstTempMat.clone();
    return true;
}

/**************** 2.图像直接卷积实现Sobel ********************/


