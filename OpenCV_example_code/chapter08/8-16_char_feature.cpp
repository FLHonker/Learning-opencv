// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-16_char_feature.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 字符特征提取
# @Create Time: 2018-10-31 13:18:55
# @Last Modified: 2018-10-31 13:18:55
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

Mat char_feature(Mat src)
{
    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
    // 特征字符尺寸归一化
    resize(gray, gray, Size(16,32));
    // 直方图均衡
    equalizeHist(gray, gray);
    gray.convertTo(gray, CV_32FC1);
    gray /= norm(gray, NORM_L2);
    // Sobel边缘检测
    Mat sobel_v_kernel = (Mat_<float>(3,3) << -0.125, 0, 0.125,
                                              -0.25, 0, 0.25,
                                              -0.125, 0, 0.125);
    Mat sobel_h_kernel = (Mat_<float>(3,3) << -0.125, -0.25, -0.125,
                                              0, 0, 0, 
                                              0.125, 0.25, 0.125);
    Mat h_edges, v_edges;
    // 卷积操作
    filter2D(gray, h_edges, gray.type(), sobel_h_kernel, Point(-1,-1), 0, BORDER_CONSTANT);
    filter2D(gray, v_edges, gray.type(), sobel_v_kernel, Point(-1,-1), 0, BORDER_CONSTANT);
    // 计算梯度模值及方向矩阵
    Mat magnitude = Mat(h_edges.size(), CV_32FC1);
    Mat angle = Mat(h_edges.size(), CV_32FC1);
    cartToPolar(v_edges, h_edges, magnitude, angle);
    // 将8个方向范围梯度的模长分别放入8个矩阵中，
    // 8个矩阵的首指针分别保存在eight_ptr[8]之中
    Mat eight_direction[8];
    float* eight_ptr[8];
    float thre[9] = {0, CV_PI/4, CV_PI/2, CV_PI*3/4, CV_PI,
                    CV_PI*5/4, CV_PI*6/4, CV_PI*7/4, CV_PI*2};
    for(int i = 0; i < 8; ++i)
    {
        eight_direction[i] = Mat::zeros(h_edges.size(), CV_32FC1);;
        eight_ptr[i] = (float*)eight_direction[i].data;
    }
    // 计算方向及梯度
    float* ang_ptr = (float*)angle.data;
    float* mag_ptr = (float*)magnitude.data;
    for(int i = 0; i < h_edges.total(); ++i, ++ang_ptr, ++mag_ptr)
    {
        for(int j = 0; j < 8; ++j)
        {
            if(*ang_ptr >= thre[i] && *ang_ptr < thre[i+1])
                *eight_ptr[i] = *mag_ptr;
            ++eight_ptr[i];
        }
    }
    // 统计8个模长矩阵的4x2个子块每个的模长之和，
    // 共计8x8=64维特征向量
    Mat feature = Mat::zeros(1, 112, CV_32FC1);
    float* fea_ptr = (float*)feature.data;
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            for(int k = 0; k < 4; ++k)
            {
                Rect roi;
                roi.x = j * 8;
                roi.y = k * 8;
                roi.width = roi.height = 8;
                *(fea_ptr++) = sum(eight_direction[i](roi).val[0]);
            }
        }
    }
    // 字符的水平投影32维及垂直投影16维向量，共计112维特征
    Mat proj_row, proj_col;
    reduce(gray, proj_row, 0, CV_REDUCE_SUM);
    reduce(gray, proj_col, 1, CV_REDUCE_SUM);
    float* tp = (float*)feature.data;
    float* dd = (float*)proj_row.data;
    for(int i = 64; i < 80; ++i)
    {
        tp[i] = dd[i - 64];
    }
    feature.colRange(80, 112) = proj_col.t();
    return feature;
}
