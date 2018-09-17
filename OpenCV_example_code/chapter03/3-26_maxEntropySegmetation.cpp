// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-26_maxEntropySegmetation.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 最大熵阈值分割
# @Create Time: 2018-09-17 16:09:01
# @Last Modified: 2018-09-17 16:09:01
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 计算当前位置的能量熵
float calcCurrentEntropy(Mat hist, int threshold)
{
    float backgroundSum = 0, targetSum = 0;
    const float* pDataHist = (float*)hist.ptr<float>(0);
    for(int i = 0; i < 256; ++i)
    {
        // 累计背景值
        if(i < threshold)
            backgroundSum += pDataHist[i];
        else    // 累计目标值
            targetSum += pDataHist[i];
    }
    //std::cout << backgroundSum << ", " << targetSum << std::endl;
    float backgroundEntropy = 0, targetEntropy = 0;
    for(int i = 0; i < 256; ++i)
    {
        if(pDataHist[i] == 0)
            continue;
        // 计算背景熵
        if(i < threshold)
        {
            float ratio1 = pDataHist[i] / backgroundSum;
            // 计算当前能量熵
            backgroundEntropy += -ratio1 * log10(ratio1);
        } else  // 计算目标熵
        {
            float ratio2 = pDataHist[i] / targetSum;
            targetEntropy += -ratio2 * log10(ratio2);
        }
    }
    return targetEntropy + backgroundEntropy;
}

// 寻找最大熵阈值并分割
Mat maxEntropySegmetation(Mat inputImg)
{
    // 初始化直方图参数
    const int channels[1] = { 0 };
    const int histSize[1] = { 256 };
    float pranges[2] = { 0, 256 };
    const float* ranges[1] = { pranges };
    MatND hist;
    // 计算直方图
    calcHist(&inputImg, 1, channels, Mat(), hist, 1, histSize, ranges);
    float max_entropy = 0;
    int max_index = 0;
    Mat resultImg;
    // 遍历得到最大熵阈值分割的最佳阈值
    for(int i = 0; i < 256; ++i)
    {
        float cur_entropy = calcCurrentEntropy(hist, i);
        // 计算当前最大值的位置
        if(cur_entropy > max_entropy)
        {
            max_entropy = cur_entropy;
            max_index = i;
        }
    }
    // 二值化分割
    threshold(inputImg, resultImg, max_index, 255, CV_THRESH_BINARY);
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/mountainwater.jpg");
    if(!srcImg.data)
        return -1;
    Mat grayImg;
    cvtColor(srcImg, grayImg, CV_BGR2GRAY);
    // 最大熵阈值分割实现
    Mat resultImg = maxEntropySegmetation(grayImg);
    imshow("grayImage", grayImg);
    imshow("resultImage", resultImg);
    waitKey(0);
    return 0;
}
