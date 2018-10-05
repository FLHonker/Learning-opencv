// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-13_GaussianBlur.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 高斯滤波
# @Create Time: 2018-10-05 17:07:16
# @Last Modified: 2018-10-05 17:07:16
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// 高斯平滑
void myGaussianBlur(const Mat& src, Mat& dst, int besarKernel, double delta)
{
    // 高斯核半径
    int kerR = besarKernel / 2;
    // 定义高斯核因子
    Mat kernel = Mat_<double>(besarKernel, besarKernel);
    // 归一化参数
    double alpha = 1 / (2 * (22 / 7) * delta * delta);
    // 生成核函数
    for(int  i = -kerR; i <= kerR; ++i)
        for(int j = -kerR; j <= kerR; ++j)
            kernel.at<double>(i+kerR, j+kerR) = exp(-((i*i + j*j)/(delta * delta * 2))) * alpha;

    dst = src.clone();
    double pix;
    // 遍历图像
    for(int i = kerR; i < src.rows - kerR; ++i)
    {
        for(int j = kerR; j < src.cols - kerR; ++j)
        {
            // 滑窗搜索完成高斯核平滑
            pix = 0;
            for(int k = -kerR; k <= kerR; ++k)
                for(int n = -kerR; n <= kerR; ++n)
                    pix += src.at<uchar>(i+k, j+n) * kernel.at<double>(kerR+k, kerR+n);

            dst.at<uchar>(i-kerR, j-kerR) = pix;
        }
    }
}

// test
int main()
{
    Mat srcImg = imread("../../imgdata/mountainwater.jpg", 1);
    if(srcImg.empty())
        return -1;
    Mat dstImg;
    myGaussianBlur(srcImg, dstImg, 5, 0.7);
    imshow("srcImage", srcImg);
    imshow("GaussianBlurImage", dstImg);
    waitKey(0);
    return 0;
}
