// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-21_linearTransform.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 线性变换 - g(x,y) = a*f(x,y)+b
#               a 表示图像对比度变化，b 表示亮度变化
# @Create Time: 2018-09-17 09:40:20
# @Last Modified: 2018-09-17 09:40:20
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

Mat linearTransform(Mat srcImg, float a, int b)
{
    if(srcImg.empty())
    {
        std::cout << "No data!" << std::endl;
    }
    const int nRows = srcImg.rows;
    const int nCols = srcImg.cols;
    Mat resultImg =Mat::zeros(srcImg.size(), srcImg.type());
    // 图像元素遍历
    for(int i = 0; i < nRows; ++i)
        for(int j = 0; j < nCols; ++j)
            for(int c = 0; c < 3; ++c)
                // 矩阵at操作，检查下标防止越界
                resultImg.at<Vec3b>(i,j)[c] = static_cast<uchar>(a * srcImg.at<Vec3b>(i,j)[c] + b);
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/butterfly.jpg");
    if(!srcImg.data)
        return -1;
    imshow("srcImage", srcImg);
    waitKey(0);
    // 线性变换
    float a = 1.2;
    int b = 50;
    Mat dstImg = linearTransform(srcImg, a, b);
    imshow("dstImage", dstImg);
    waitKey(0);
    return 0;
}
