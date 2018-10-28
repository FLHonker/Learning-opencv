// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-10_LBP_classic.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 经典LBP特征
# @Create Time: 2018-10-28 12:51:12
# @Last Modified: 2018-10-28 12:51:12
***********************************************/
template <typename _Tp> static
void olbp_(InputtArray _src, OutputArray _dst)
{
    // 得到矩阵信息
    Mat src = _src.getMat();
    // 输出结果矩阵
    _dst.create(src.rows-2, src.cols-2, CV_8UC1);
    Mat dst = _dst.getMat();
    // 初始化矩阵
    dst.setTo(0);
    // 遍历计算局部特征
    for(int i = 1; i < src.rows-1; ++i)
    {
        for(int j = 1; j < src.cols-1; ++j)
        {
            // 获取当前图像处理的中心点
            _Tp center = src.at<_Tp>(i, j);
            unsigned char code = 0;
            // 比较图像编码形成
            code |= (src.at<_Tp>(i-1,j-1) >= center) << 7;
            code |= (src.at<_Tp>(i-1,j) >= center) << 6;
            code |= (src.at<_Tp>(i-1,j+1) >= center) << 5;
            code |= (src.at<_Tp>(i,j+1) >= center) << 4;
            code |= (src.at<_Tp>(i+1,j+1) >= center) << 3;
            code |= (src.at<_Tp>(i+1,j) >= center) << 2;
            code |= (src.at<_Tp>(i+1,j-1) >= center) << 1;
            code |= (src.at<_Tp>(i,j-1) >= center) << 0;
            dst.at<unsigned char>(i-1, j-1) = code;
        }
    }
}
