// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-7_convolution.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 图像卷积操作
# @Create Time: 2018-10-03 19:39:31
# @Last Modified: 2018-10-03 19:39:31
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// 图像卷积操作
/**
 * @brief 
 *
 * @param src
 * @param kernel
 * @param dst
 */
void convolution(Mat src, Mat kernel, Mat& dst)
{
    // 输出图像定义
    dst.create(abs(src.rows - kernel.rows)+1, abs(src.cols - kernel.cols)+1, src.type());
    Size dftSize;
    // 计算傅里叶变换尺寸
    dftSize.width = getOptimalDFTSize(src.cols + kernel.cols - 1);
    dftSize.height = getOptimalDFTSize(src.rows + kernel.rows - 1);
    // 创建临时图像，初始化为0
    Mat tempA(dftSize, src.type(), Scalar::all(0));
    Mat tempB(dftSize, kernel.type(), Scalar::all(0));
    // 对区域进行复制
    Mat roiA(tempA, Rect(0, 0, src.cols, src.rows));
    src.copyTo(roiA);
    Mat roiB(tempB, Rect(0, 0, kernel.cols, kernel.rows));
    kernel.copyTo(roiB);
    // 傅里叶变换
    dft(tempA, tempA, 0, src.rows);
    dft(tempB, tempB, 0, kernel.rows);
    // 对频谱中的每个元素进行乘法操作
    mulSpectrums(tempA, tempB, tempA, DFT_COMPLEX_OUTPUT);
    // 交换结果，所有行非零
    dft(tempA, tempA, DFT_INVERSE + DFT_SCALE, dst.rows);
    // 复制结果到输出图像
    tempA(Rect(0, 0, dst.cols, dst.rows)).copyTo(dst);
}

int main()
{
    Mat srcImg = imread("../../imgdata/building2.jpg", 0);
    if(srcImg.empty())
        return -1;
    // 定义卷积核算子
    Mat kernel = (Mat_<float>(3, 3) <<  1, 1, 1,
                                        1, 1, 1,
                                        1, 1, 1);
    imshow("srcImage", srcImg);
    srcImg.convertTo(srcImg, CV_32F);
    Mat resultImg;
    // 卷积操作
    convolution(srcImg, kernel, resultImg);
    // 归一化输出
    normalize(resultImg, resultImg, 0, 1, CV_MINMAX);
    imshow("resultImage", resultImg);
    waitKey(0);
    return 0;
}
