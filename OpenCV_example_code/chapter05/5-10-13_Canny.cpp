// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-10-13_Canny.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 改进边缘检测算子——Canny
# @Create Time: 2018-10-09 10:45:37
# @Last Modified: 2018-10-09 10:45:37
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// 边缘连接
void followEdges(int x, int y, Mat &magnitude, int tUpper,
                 int tLower, Mat &edges)
{
    edges.at<float>(y, x) = 255;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            // 边界限制
            if ((i != 0) && (j != 0) && (x + i >= 0) &&
                (y + j >= 0) && (x + i < magnitude.cols) &&
                (y + j < magnitude.rows)) {
                // 梯度幅值边缘判断及连接
                if ((magnitude.at<float>(y + j, x + i) > tLower)
                    && (edges.at<float>(y + j, x + i) != 255)) {
                    followEdges(x + i, y + j, magnitude,
                                tUpper, tLower, edges);
                }
            }
        }
    }
}
// 边缘检测
void edgeDetect(Mat &magnitude, int tUpper, int tLower,
                Mat &edges)
{
    int rows = magnitude.rows;
    int cols = magnitude.cols;
    edges = Mat(magnitude.size(), CV_32F, 0.0);

    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            // 梯度幅值判断
            if (magnitude.at<float>(y, x) >= tUpper) {
                followEdges(x, y, magnitude, tUpper,
                            tLower, edges);
            }
        }
    }
}
// 非极大值抑制
void nonMaximumSuppression(Mat &magnitudeImage,
                           Mat &directionImage)
{
    Mat checkImage = Mat(magnitudeImage.rows, magnitudeImage.cols, CV_8U);
    // 迭代器初始化
    MatIterator_<float>itMag = magnitudeImage.begin<float>();
    MatIterator_<float>itDirection = directionImage.begin <float>();
    MatIterator_<unsigned char>itRet = checkImage.begin<unsigned char>();
    MatIterator_<float>itEnd = magnitudeImage.end<float>();

    // 计算对应方向
    for (; itMag != itEnd; ++itDirection, ++itRet, ++itMag) 
    {
        // 将方向进行划分， 对每个方向进行幅值判断
        const Point pos = itRet.pos();
        float currentDirection = atan(*itDirection) * (180 / 3.142);

        while (currentDirection < 0)
            currentDirection += 180;

        *itDirection = currentDirection;

        // 边界限定,对相应方向进行判断
        if (currentDirection > 22.5 && currentDirection <= 67.5) {
            // 邻域位置极值判断
            if (pos.y > 0 && pos.x > 0 && *itMag <=
                magnitudeImage.at<float>(pos.y - 1, pos.x - 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }

            if (pos.y < magnitudeImage.rows - 1 && pos.x <
                magnitudeImage.cols - 1 && *itMag <=
                magnitudeImage.at<float>(pos.y + 1, pos.x + 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        } else if (currentDirection > 67.5 && currentDirection <= 112.5) {
            // 邻域位置极值判断
            if (pos.y > 0 && *itMag <= magnitudeImage.at<float>(pos.y - 1, pos.x)) 
            {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }

            if (pos.y < magnitudeImage.rows - 1 &&
                *itMag <= magnitudeImage.at<float>(pos.y + 1, pos.x)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        } else if (currentDirection > 112.5 && currentDirection <= 157.5) {
            // 邻域位置极值判断
            if (pos.y > 0 && pos.x < magnitudeImage.cols - 1 &&
                *itMag <= magnitudeImage.at<float>(pos.y - 1,
                                                   pos.x + 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;;
            }

            if (pos.y < magnitudeImage.rows - 1 && pos.x > 0 &&
                *itMag <= magnitudeImage.at<float>(pos.y + 1,
                                                   pos.x - 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        } else {
            // 邻域位置极值判断
            if (pos.x > 0 && *itMag <=
                magnitudeImage.at<float>(pos.y, pos.x - 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }

            if (pos.x < magnitudeImage.cols - 1 && *itMag <=
                magnitudeImage.at<float>(pos.y, pos.x + 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        }
    }
}

void myCanny(Mat& src, Mat& edges, int upperThresh, int lowerThresh)
{
    /************ 1. 消除噪声 **************/

    // 高斯滤波
    Mat image;
    GaussianBlur(src, image, Size(3,3), 1.5);

    /************ 2. 计算梯度幅值与方向 *************/

    // 使用Sobel计算相应的梯度幅值及方向
    Mat magX = Mat(src.rows, src.cols, CV_32F);
    Mat magY = Mat(src.rows, src.cols, CV_32F);
    Sobel(image, magX, CV_32F, 1, 0, 3);
    Sobel(image, magY, CV_32F, 0, 1, 3);
    // 计算斜率
    Mat slopes = Mat(image.rows, image.cols, CV_64F);
    divide(magY, magX, slopes);
    // 计算每个点的梯度
    Mat sum = Mat(image.rows, image.cols, CV_64F);
    Mat prodX = Mat(image.rows, image.cols, CV_64F);
    Mat prodY = Mat(image.rows, image.cols, CV_64F);
    multiply(magX, magX, prodX);
    multiply(magY, magY, prodY);
    sum = prodX + prodY;
    sqrt(sum, sum);
    Mat magnitude = sum.clone();

    
    /*************** 3. 非极大值抑制 ****************/

    nonMaximumSuppression(magnitude, slopes);

    /*************** 4. 滞后阈值边缘连接 *************/
    
    edgeDetect(magnitude, upperThresh, lowerThresh, edges);
}

int main()
{
    Mat srcImage = imread("../../imgdata/building.jpg", 0);

    if (!srcImage.data)
        return -1;

    int highValue = 100;
    int lowValue = 50;
    Mat cannyEdges;
    myCanny(srcImage, cannyEdges, highValue, lowValue);
    imshow("srcImage", srcImage);
    imshow("cannyEdges", cannyEdges);
    waitKey();
    return 0;
}
