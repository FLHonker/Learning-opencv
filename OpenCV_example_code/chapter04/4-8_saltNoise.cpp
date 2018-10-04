// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-8_saltNoise.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 椒盐噪声
# @Create Time: 2018-10-03 20:46:03
# @Last Modified: 2018-10-03 20:46:03
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <cstdlib>

using namespace cv;

// 图像加椒盐噪声
Mat addSaltNoise(const Mat srcImg, int n)
{
    Mat resultImg = srcImg.clone()    ;
    for(int k = 0; k < n; ++k)
    {
        // 随机取值行列
        int i = rand() % resultImg.rows;
        int j = rand() % resultImg.cols;
        // 图像通道判定
        if(resultImg.channels() == 1)
        {
            resultImg.at<uchar>(i,j) = 255;
        } else {
            resultImg.at<Vec3b>(i,j)[0] = 255;
            resultImg.at<Vec3b>(i,j)[1] = 255;
            resultImg.at<Vec3b>(i,j)[2] = 255;
        }
    }
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/building2.jpg");
    if(srcImg.empty())
        return -1;
    Mat resultImg = addSaltNoise(srcImg, 5000);
    imshow("srcImage", srcImg);
    imshow("saltImage", resultImg);
    waitKey(0);
    return 0;
}
