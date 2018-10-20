// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-4_MeanShift.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 均值漂移MeanShift
# @Create Time: 2018-10-18 21:45:03
# @Last Modified: 2018-10-18 21:45:03
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// 颜色通道分离合并
static void MergeSeg(Mat& img, const Scalar& colorDiff = Scalar::all(1))
{
    CV_Assert(!img.empty());
    RNG rng = theRNG();
    // 定义掩码图像
    Mat mask(img.rows+2, img.cols+2, CV_8UC1, Scalar::all(0));
    for(int x = 0; x < img.rows; ++x)
    {
        for(int y = 0; y < img.cols; ++y)
        {
            if(mask.at<uchar>(x+1, y+1) == 0)
            {
                // 定义颜色
                Scalar newVal(rng(256), rng(256), rng(256));
                // 泛洪合并
                floodFill(img, mask, Point(y, x), newVal, 0, colorDiff, colorDiff);
            }
        }
    }
}

int main()
{
    Mat srcImg = imread("../../imgdata/sea.jpg");
    if(srcImg.empty())
        return -1;
    // 设定参数
    int spatialRad = 20, colorRad = 20, maxPyrLevel = 6;
    Mat dstImg;
    // MeanShift
    pyrMeanShiftFiltering(srcImg, dstImg, spatialRad, colorRad, maxPyrLevel);
    imshow("dstImage", dstImg);
    // 颜色通道分离合并
    MergeSeg(dstImg, Scalar::all(2));
    imshow("srcImage", srcImg);
    imshow("dst merged", dstImg);
    waitKey(0);
    return 0;
}
