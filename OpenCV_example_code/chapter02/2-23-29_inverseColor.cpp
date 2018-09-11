// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-23-29_inverseColor.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 5中反色方法对比
# @Create Time: 2018-09-10 11:15:45
# @Last Modified: 2018-09-10 11:15:45
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// 2-23 初始化Hilbert矩阵
Mat H = imread("../../imgdata/lena.jpg");   // 全局变量
void initHilbert(Mat& H)
{
    for(int i = 0; i < H.rows; ++i)
        for(int j = 0; j < H.cols; ++j)
            H.at<double>(i, j) = 1.0 / (i + j +1);
}

// 2-24 方法1：下标M.at<float>(i,j)
Mat inverseColor1(Mat srcImg)
{
    Mat tempImg = srcImg.clone();
    int row = tempImg.rows;
    int col = tempImg.cols;
    // 对各个像素点遍历进行取反
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j)
        {
            // 分别对各个通道进行反色处理
            tempImg.at<Vec3b>(i,j)[0] = 255 - tempImg.at<Vec3b>(i,j)[0];
            tempImg.at<Vec3b>(i,j)[1] = 255 - tempImg.at<Vec3b>(i,j)[1];
            tempImg.at<Vec3b>(i,j)[2] = 255 - tempImg.at<Vec3b>(i,j)[2];
        }
    }
    return tempImg;
}

// 2-25 方法2：指针遍历Mat::ptr<type>
Mat inverseColor2(Mat srcImg)
{
    Mat tempImg = srcImg.clone();
    int row = tempImg.rows;
    // 将三通道转换为单通道
    int nStep = tempImg.cols * tempImg.channels();
    for(int  i = 0; i < row; ++i)
    {
        // 取源图像的指针
        const uchar* pSrcData = srcImg.ptr<uchar>(i);
        // 将输出数据指针存放为输出图像
        uchar* pResultData = tempImg.ptr<uchar>(i);
        for(int j = 0; j < nStep; ++j)
        {
            pResultData[j] = cv::saturate_cast<uchar>(255 - pSrcData[j]);
        }
    }
    return tempImg;
}

// 2-26 方法3：使用迭代器MatIterator_
Mat inverseColor3(Mat srcImg)
{
    Mat tempImg = srcImg.clone();
    // 初始化源图像迭代器
    MatConstIterator_<Vec3b> srcIterStart = srcImg.begin<Vec3b>();
    MatConstIterator_<Vec3b> srcIterEnd = srcImg.end<Vec3b>();
    // 初始化输出图像迭代器
    MatIterator_<Vec3b> outIterStart = tempImg.begin<Vec3b>();
    MatIterator_<Vec3b> outIterEnd = tempImg.end<Vec3b>();
    
    // 遍历图像反色处理
    while(srcIterStart != srcIterEnd)
    {
        (*outIterStart)[0] = 255 - (*srcIterStart)[0];
        (*outIterStart)[1] = 255 - (*srcIterStart)[1];
        (*outIterStart)[2] = 255 - (*srcIterStart)[2];
        // 迭代器递增
        srcIterStart++;
        outIterStart++;
    }
    return tempImg;
}

// 2-27 方法4：改进方法isContinuous
Mat inverseColor4(Mat srcImg)
{
    int row = srcImg.rows;
    int col = srcImg.cols;
    Mat tempImg = srcImg.clone();
    // 判断是否是连续图像，即是否有像素填充
    if(srcImg.isContinuous() && tempImg.isContinuous())
    {
        row = 1;    // 转换为一行
        // 按照行展开
        col = col * srcImg.rows * srcImg.channels();
        // col = col * srcImg.step;
    }
    // 遍历图像的每个像素
    for(int i = 0; i < row; ++i)
    {
        // 设定图像数据源指针及输出图像数据指针
        const uchar* pSrcData = srcImg.ptr<uchar>(i);
        uchar* pResultData = tempImg.ptr<uchar>(i);
        for(int j = 0; j < col; ++j)
            *pResultData++ = 255 - *pSrcData++;
    }
    return tempImg;
}

// 2-28 方法5：LUT查表法
Mat inverseColor5(Mat srcImg)
{
    int row = srcImg.rows;
    int col = srcImg.cols;
    Mat tempImg = srcImg.clone();
    // 建立LUT反色table
    uchar LutTable[256];
    for(int i = 0; i < 256; ++i)
        LutTable[i] = 255 - i;
    Mat lookUpTable(1, 256, CV_8U);
    uchar* pData = lookUpTable.data;
    // 建立映射表
    for(int i = 0; i < 256; ++i)
        pData[i] = LutTable[i];
    // 应用索引表进行查找
    LUT(srcImg, lookUpTable, tempImg);

    return tempImg;
}

// 2-29 算法计时比较
int main()
{
    // Mat H(100, 100, CV_64F);
    // initHilbert(H);
    const int nTimes = 100;
    Mat outImg(H.rows, H.cols, H.type());
    // test-1
    double tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
    {
        outImg = inverseColor1(H);
    }
    imshow("inverseColor1", outImg);
    waitKey(1000);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    cout << "M.at<>: " << tTime << endl;

    // test-2
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
    {
        outImg = inverseColor2(H);
    }
    imshow("inverseColor2", outImg);
    waitKey(1000);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    cout << "Mat::ptr<type>: " << tTime << endl;

    // test-3
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
    {
        outImg = inverseColor3(H);
    }
    imshow("inverseColor3", outImg);
    waitKey(1000);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    cout << "MatIterator: " << tTime << endl;
    
    // test-4
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
    {
        outImg = inverseColor4(H);
    }
    imshow("inverseColor4", outImg);
    waitKey(1000);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    cout << "改进的isContinuous: " << tTime << endl;

    // test-5
    tTime = (double)getTickCount();
    for(int i = 0; i < nTimes; ++i)
    {
        outImg = inverseColor5(H);
    }
    imshow("inverseColor5", outImg);
    waitKey(1000);
    tTime = 1000*((double)getTickCount() - tTime) / getTickFrequency();
    tTime /= nTimes;
    cout << "LUT查表法: " << tTime << endl;
    
    return 0;
}

