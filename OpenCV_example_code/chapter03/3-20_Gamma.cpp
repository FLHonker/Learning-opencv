// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-20_Gamma.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Gamma校正
# @Create Time: 2018-09-16 19:29:52
# @Last Modified: 2018-09-16 19:29:52
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// Gamma变换函数实现
Mat gammaTransform(Mat& srcImg, float kFactor)
{
    // 建立查找表LUT
    unsigned char LUT[256];
    for(int i = 0; i < 256; ++i)
    {
        // Gamma变换表达式
        LUT[i] = saturate_cast<uchar>(pow((float)i/255.0, kFactor) * 255.0f);
    }
    Mat resultImg = srcImg.clone();
    // 输入通道为单通道时，直接进行变换
    if(srcImg.channels() == 1)
    {
        MatIterator_<uchar> iter = resultImg.begin<uchar>();
        MatIterator_<uchar> iterEnd = resultImg.end<uchar>();
        for(; iter != iterEnd; ++iter)
            *iter = LUT[*iter];
    } else {
        // 输入为3通道时，需对每个通道分别进行变换
        MatIterator_<Vec3b> iter = resultImg.begin<Vec3b>() ;
        MatIterator_<Vec3b> iterEnd = resultImg.end<Vec3b>();
        for(; iter != iterEnd; ++iter)
        {
            (*iter)[0] = LUT[(*iter)[0]];
            (*iter)[1] = LUT[(*iter)[1]];
            (*iter)[2] = LUT[(*iter)[2]];
        }
    }
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/butterfly.jpg");
    if(!srcImg.data)
        return -1;
    // 初始化两种不同参数，对比分析效果
    float kFactor1 = 0.3, kFactor2 = 3.0;
    Mat result1 = gammaTransform(srcImg, kFactor1);
    Mat result2 = gammaTransform(srcImg, kFactor2);
    imshow("srcImage", srcImg);
    imshow("result1", result1);
    imshow("result2", result2);
    waitKey(0);
    return 0;
}
