// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-17_videoQuality.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 视频质量评价：PSNR（峰值信噪比），SSIM（结构相似性）
# @Create Time: 2018-09-08 10:59:27
# @Last Modified: 2018-09-08 10:59:27
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 计算PSNR峰值信噪比，返回数值为30~50dB，值越大越好
double PSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    // 计算图像差|I1 - I2|
    absdiff(I1, I2, s1);
    // 转成32浮点数进行平方运算
    s1.convertTo(s1, CV_32F);
    // s1*s1，即|I1 - I2|^2
    s1 = s1.mul(s1);
    // 分别叠加每个通道的元素，存于s中
    Scalar s = sum(s1);
    // 计算所有通道元素和
    double sse = s.val[0] + s.val[1] + s.val[2];
    // 当元素很小时返回0
    if(sse <= 1e-10)
        return 0;
    else
    {
        // 根据公式计算当前I1与I2的均方误差
         double mse = sse / (double)(I1.channels() * I1.total());
         // 计算峰值信噪比
         double psnr = 10.0 * log10(255*255/sse);
         return psnr;
    }
}

// 计算MSSIM结构相似性，返回值0～1，值越大越好
cv::Scalar MSSIM(const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    Mat I1, I2;
    // 转换成32位浮点数进行平方运算
    i1.convertTo(I1, CV_32F);
    i2.convertTo(I2, CV_32F);
    // I1^2, I2^2, I1*I2
    Mat I1_2 = I1.mul(I1);
    Mat I2_2 = I2.mul(I2);
    Mat I1_I2 = I1.mul(I2);
    Mat mu1, mu2;
    // 高斯加权计算每一窗口的均值、方差以及协方差
    GaussionBlur(I1, mu1, Size(11,11), 1.5);
    GaussionBlur(I2, mu2, Size(11,11), 1.5);
    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);
    Mat sigma1_2, sigma2_2, sigma12;
    // 高斯平滑，具体原理见4.4
    GaussionBlur(I1_2, sigma1_2, Size(11,11), 1.5);
    sigma1_2 -= mu1_2;
    GaussionBlur(I2_2, sigma2_2, Size(11,11), 1.5);
    sigma2_2 -= mu2_2;
    GaussionBlur(I1_I2, sigma12, Size(11,11), 1.5);
    sigma12 -= mu1_mu2;
    // 根据公式计算相应参数
    Mat t1, t2, t3;
    t1 += 2 * mu1_mu2 + C1;
    t2 += 2 * sigma12 + C2;
    // t3 = (2*mu1_mu2 + C1).*(2*sigma12 + C2);
    t3 = t1.mul(t2);
    t1 = mu1_2 + mu2_2 + C2;
    t2 = sigm1_2 + sigma2_2 + C2;
    // t1 = (mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2)
    t1 = t1.mul(t2);
    Mat ssim_map;
    // ssim_map = t3./t1
    divide(t3, t1, ssim_map);
    // 将平均值作为两图像的结构相似性度量
    Scalar mssim = mean(ssim_map);

    return mssim;
}
