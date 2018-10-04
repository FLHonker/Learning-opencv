// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-11_blur.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 均值滤波实现
# 均值滤波对周期性的干扰噪声有很好的抑制作用。
# @Create Time: 2018-10-04 21:38:12
# @Last Modified: 2018-10-04 21:38:12
***********************************************/
// 均值滤波器实现
void cv::blur(InputArray src, OutputArray dst,
    Size ksize, Point anchor, int borderType)
{
    // 调用盒滤波器
    boxFilter(src, dst, -1, ksize, anchor, true, borderType);
}
