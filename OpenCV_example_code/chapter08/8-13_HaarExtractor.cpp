// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-13_HaarExrtactor.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Haar特征提取
# @Create Time: 2018-10-30 11:04:26
# @Last Modified: 2018-10-30 11:04:26
***********************************************/
#include <opencv2/opencv.hpp>

using namespace cv;

// 计算单窗口的积分图
double calcIntegral(double const **image, int x, int y, int width, int height)
{
    double term_1 = image[y - 1 + height][x - 1 + width];
    double term_2 = image[y - 1][x - 1];
    double term_3 = image[y - 1 + height][x - 1];
    double term_4 = image[y - 1][x - 1 + width];
    return (term_1 + term_2) - (term_3 + term_4);
}

// 计算Haar特征
double HarrExtractor(double const **image, int type_, Rect roi)
{
    double value;
    double wh1;
    double bk1, bk2;
    int x = roi.x, y = roi.y;
    int width = roi.width, height = roi.height;
    switch(type_)
    {
    // Haar水平边缘
    case 0:
        wh1 = calcIntegral(image, x, y, width, height);
        bk1 = calcIntegral(image, x + width, y, width, height);
        value = (wh1 - bk1) / static_cast<double>(width * height);
        break;
    // Haar竖直边缘
    case 1:
        wh1 = calcIntegral(image, x, y, width, height);
        bk1 = calcIntegral(image, x, y + height, width, height);
        value = (wh1 - bk1) / static_cast<double>(width * height);
        break;
    // Haar水平线型
    case 2:
        wh1 = calcIntegral(image, x, y, width * 3, height);
        bk1 = calcIntegral(image, x + width, y, width, height);
        value = (wh1 - 3.0 * bk1) / static_cast<double>(2 * width * height);
        break;
    // Haar垂直线型
    case 3:
        wh1 = calcIntegral(image, x, y, width, height * 3);
        bk1 = calcIntegral(image, x, y + height, width, height);
        value = (wh1 - 3.0 * bk1) / static_cast<double>(2 * width * height);
        break;
    // Haar棋盘型
    case 4:
        wh1 = calcIntegral(image, x, y, width * 2, height * 2);
        bk1 = calcIntegral(image, x + width, y, width, height);
        bk2 = calcIntegral(image, x, y + height, width, height);
        value = (wh1 - 2.0 * (bk1 + bk2)) / static_cast<double>(2 * width * height);
        break;
    // Haar中心包围型
    case 5:
        wh1 = calcIntegral(image, x, y, width * 3, height * 3);
        bk1 = calcIntegral(image, x + width, y + height, width, height);
        value = (wh1 - 9.0 * bk1) / static_cast<double>(8 * width * height);
        break;
    }
    return value;
}

