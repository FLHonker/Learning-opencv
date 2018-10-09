// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-14_cvCanny.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: Canny库函数实现
# @Create Time: 2018-10-09 15:14:09
# @Last Modified: 2018-10-09 15:14:09
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(!srcImg.data)
        return -1;
    Mat resultImg;
    int edgeThresh = 50;
    // Canny检测
    Canny(srcImg, resultImg, edgeThresh, edgeThresh*3, 3);
    imshow("srcImage", srcImg);
    imshow("resultImage", resultImg);
    waitKey(0);
    return 0;
}
