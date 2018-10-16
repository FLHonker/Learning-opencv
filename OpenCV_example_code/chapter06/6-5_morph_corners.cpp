// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 6-5_morph_corners.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 形态学角点检测
# @Create Time: 2018-10-16 10:20:36
# @Last Modified: 2018-10-16 10:20:36
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/sea.jpg");
    if(srcImg.empty())
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_RGB2BGRA);
    // 定义结构元素
    Mat crossMat(5, 5, CV_8U, Scalar(0));
    Mat diamonMat(5, 5, CV_8U, Scalar(1));
    Mat squareMat(5, 5, CV_8U, Scalar(1));
    Mat XMat(5, 5, CV_8U, Scalar(0));
    // 十字形状
    for(int i = 0; i < 5; ++i)
    {
        crossMat.at<uchar>(i, 2) = 1;
        crossMat.at<uchar>(2, i) = 1;
    }
    // 菱形形状
    diamonMat.at<uchar>(0, 0) = 0;
    diamonMat.at<uchar>(0, 1) = 0;
    diamonMat.at<uchar>(1, 0) = 0;
    diamonMat.at<uchar>(4, 4) = 0;
    diamonMat.at<uchar>(3, 4) = 0;
    diamonMat.at<uchar>(4, 3) = 0;
    diamonMat.at<uchar>(4, 0) = 0;
    diamonMat.at<uchar>(4, 1) = 0;
    diamonMat.at<uchar>(3, 0) = 0;
    diamonMat.at<uchar>(0, 4) = 0;
    diamonMat.at<uchar>(0, 3) = 0;
    diamonMat.at<uchar>(1, 4) = 0;
    // X形状
    for(int i = 0; i < 5; ++i)
    {
        XMat.at<uchar>(i, i) = 1;
        XMat.at<uchar>(4 - i, i) = 1;
    }
    // 第1步：十字形对原图进行膨胀,边缘扩展，角点不变
    Mat result;
    dilate(srcGray, result, crossMat);
    // 第2步：菱形对上步进行腐蚀，边缘不变，角点收缩
    erode(result, result, diamonMat);
    imshow("result1", result);
    // 第3步：X形对原图进行膨胀，角点扩展
    Mat result2;
    dilate(srcGray, result2, XMat);
    // 第4步：正方形对上步进行腐蚀，角点恢复原状，边缘腐蚀
    erode(result2, result2, squareMat);
    imshow("result2", result2);
    // 第5步：计算差值
    absdiff(result2, result, result);
    threshold(result, result, 40, 255, THRESH_BINARY);
    // 绘图
    for(int i = 0; i < result.rows; ++i)
    {
        // 获取行指针
        const uchar* pData = result.ptr<uchar>(i);
        for(int j = 0; j < result.cols; ++j)
        {
            // 如果是角点，则进行绘图
            if(pData[j])
                circle(srcImg, Point(j,i), 5, Scalar(0,255,0));
        }
    }
    imshow("scrImage", srcImg);
    imshow("corners", result);
    waitKey(0);
    return 0;
}
