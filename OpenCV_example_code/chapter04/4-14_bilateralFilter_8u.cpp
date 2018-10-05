// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-14_bilateralFilter_8u.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 8位图像的双边滤波实现
# @Create Time: 2018-10-05 17:51:24
# @Last Modified: 2018-10-05 17:51:24
***********************************************/
static void bilateralFilter_8u(const Mat& src, Mat& dst, int d,
                               double sigma_color, double sigma_space, int borderType)
{
    // 获取原始图像信息
    int cn = src.channels();
    int i, j, maxk, radius;
    Size size = src.size();
    CV_Assert((src.type() == CV_8UC1 || src.type() == CV_8UC3) &&
              src.type() == dst.type() && src.size() == dst.size() &&
              src.data != dst.data);

    // 高斯参数设置
    if (sigma_color <= 0)
        sigma_color = 1;

    if (sigma_space <= 0)
        sigma_space = 1;

    // 颜色空间与距离空间初始化
    double gauss_color_coeff = -0.5 / (sigma_color * sigma_color);
    double gauss_space_coeff = -0.5 / (sigma_space * sigma_space);

    // 内核边界设置
    if (d <= 0)
        radius = cvRound(sigma_space * 1.5);
    else
        radius = d / 2;

    radius = MAX(radius, 1);
    d = radius * 2 + 1;
    // 图像边界处理
    Mat temp;
    copyMakeBorder(src, temp, radius, radius, radius, radius, borderType);
    // 距离空间相似度计算
    vector<float> _color_weight(cn * 256);
    vector<float> _space_weight(d * d);
    vector<int> _space_ofs(d * d);
    float* color_weight = &_color_weight[0];
    float* space_weight = &_space_weight[0];
    int* space_ofs = &_space_ofs[0];

    // 初始化颜色相似度权值
    for (i = 0; i < 256 * cn; i++)
        color_weight[i] = (float)std::exp(i * i * gauss_color_coeff);

    // 计算距离相似度权值
    for (i = -radius, maxk = 0; i <= radius; i++) {
        j = -radius;

        for (; j <= radius; j++) {
            // 计算距离范数
            double r = std::sqrt((double)i * i + (double)j * j);

            if (r > radius)
                continue;

            // 应用高斯变换计算出相似度
            space_weight[maxk] = (float)std::exp(r * r * gauss_space_coeff);
            space_ofs[maxk++] = (int)(i * temp.step + j * cn);
        }
    }

    // 得到双边滤波结果
    BilateralFilter_8u_Invoker body(dst, temp, radius, maxk,
                                    space_ofs, space_weight, color_weight);
    parallel_for_(Range(0, size.height), body, dst.total() / (double)(1 << 16));
}
