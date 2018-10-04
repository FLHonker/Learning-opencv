// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-10_boxFilter.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: boxFilter滤波器实现
# @Create Time: 2018-10-04 21:30:11
# @Last Modified: 2018-10-04 21:30:11
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 盒子滤波器基类生成函数
cv::Ptr<cv::FilterEngine> cv::createBoxFilter(int srcType, int dstType, 
    Size ksize, Point anchor, bool normalize, int borderType)
{
    // 基类参数设置，图像深度
    
}
