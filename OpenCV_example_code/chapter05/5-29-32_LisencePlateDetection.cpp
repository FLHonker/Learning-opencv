// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-29-32_LisencePlateDetection.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 车牌检测
# @Create Time: 2018-10-14 13:06:39
# @Last Modified: 2018-10-14 13:06:39
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/* 提取竖直的Sobel边缘 */
bool SobelVerEdge(Mat src, Mat& dst)
{
    CV_Assert(src.channels() == 1);
    src.convertTo(src, CV_32FC1);
    // 水平方向的Sobel算子
    Mat sobelx = (Mat_<float>(3,3) << -0.125, 0, 0.125,
                                        -0.25, 0, 0.25,
                                       -0.125, 0, 0.125);
    // 卷积运算
    Mat conResMat;
    filter2D(src, conResMat, src.type(), sobelx);
    // 计算梯度幅度
    Mat graMagMat;
    multiply(conResMat, conResMat, graMagMat);
    // 根据梯度幅度及参数设置阈值
    int scaleVal = 4;
    double thresh = scaleVal * mean(graMagMat).val[0];
    Mat dstTempMat = Mat::zeros(graMagMat.size(), graMagMat.type());
    float* pDataMag = (float*)graMagMat.data;
    float* pDataDst = (float*)dstTempMat.data;
    const int nRows = conResMat.rows;
    const int nCols = conResMat.cols;
    for(int i = 1; i < nRows - 1; ++i)
    {
        for(int j = 1; j < nCols - 1; ++j)
        {
            // 计算该店梯度与水平或垂直梯度值大小的比较结果
            bool b1 = pDataMag[i * nCols + j] > pDataMag[i * nCols + j -1];
            bool b2 = pDataMag[i * nCols + j] > pDataMag[i * nCols + j +1];
            bool b3 = pDataMag[i * nCols + j] > pDataMag[(i-1) * nCols + j];
            bool b4 = pDataMag[i * nCols + j] > pDataMag[(i+1) * nCols + j];
            // 判断邻域梯度是否满足大于水平或垂直梯度，
            // 并根据自适应阈值参数进行二值化
            pDataDst[i * nCols + j] = 255 * ((pDataMag[i * nCols + j] > thresh) && 
                                             (b1 && b2 || b3 && b4));
        }
    }
    dstTempMat.convertTo(dstTempMat, CV_8UC1);
    dst = dstTempMat.clone();
    return true;
}

/* 疑似车牌区域提取 */
vector<Mat> getPlateArea(Mat src, Mat sobelMat)
{
    // 转换成HSV
    Mat img_h, img_v, img_s, img_hsv;
    vector<Mat> hsv_vec;
    cvtColor(src, img_hsv, CV_BGR2HSV);
    imshow("hsvImage", img_hsv);
    waitKey(10000);
    // 分割HSV通道
    split(img_hsv, hsv_vec);
    img_h = hsv_vec[0];
    img_s = hsv_vec[1];
    img_v = hsv_vec[2];
    // 转换通道数据类型
    img_h.convertTo(img_h, CV_32F);
    img_s.convertTo(img_s, CV_32F);
    img_v.convertTo(img_v, CV_32F);
    // 计算每个通道的最大值
    double max_h, max_s, max_v;
    minMaxIdx(img_h, 0, &max_h);
    minMaxIdx(img_s, 0, &max_s);
    minMaxIdx(img_v, 0, &max_v);
    // 各个通道归一化
    img_h /= max_h;
    img_s /= max_s;
    img_v /= max_v;

    // HSV限定范围元素提取
    Mat bw_blue = (img_h > 0.45) & (img_h < 0.7) & (img_s > 0.15) & (img_v > 0.25);
    int height = bw_blue.rows, width = bw_blue.cols;
    Mat bw_blue_edge = Mat::zeros(bw_blue.size(), bw_blue.type());
    imshow("bw_blue", bw_blue);
    waitKey(10000);
    // 车牌疑似区域提取
    for(int k = 1; k < height - 2; ++k)
    {
        for(int m = 1; m < width - 2; ++m)
        {
            // 滑窗搜索
            Rect rct;
            rct.x = m - 1;
            rct.y = k - 1;
            rct.height = 3;
            rct.width = 3;
            // 判断当前点属于边缘且颜色区域内至少包含1个像素
            if(sobelMat.at<uchar>(k, m) == 255 && countNonZero(bw_blue(rct) >= 1))
                bw_blue_edge.at<uchar>(k, m) = 255;
        }
    }

    // 形态学闭操作
    Mat morph;
    morphologyEx(bw_blue_edge, morph, MORPH_CLOSE, Mat::ones(2, 25, CV_8UC1));
    Mat imgshow5;
    resize(bw_blue_edge, imgshow5, Size(), 1, 1);
    imshow("morphology_bw_blue_edge", imgshow5);
    waitKey(10000);
    // 连通区域提取
    imshow("morph", morph);
    // 求闭操作的连通域外轮廓
    vector<vector<Point>> region_contours;
    findContours(morph.clone(), region_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 候选轮廓筛选
    vector<Rect> candidates;
    vector<Mat> candidates_img;
    Mat plateImg;
    for(size_t i = 0; i < region_contours.size(); ++i)
    {
        // 去除高度/宽度不符合条件的区域
        Rect rect = boundingRect(region_contours[i]);
        // 计算区域非0像素点
        int sub = countNonZero(morph(rect));
        // 非0像素点占的比例
        double ratio = (double)sub / rect.area();
        // 宽高比
        double wh_ratio = (double)rect.width / rect.height;
        // 有效像素点、宽高及比例判断
        if(ratio > 0.5 && wh_ratio > 2 && wh_ratio < 5 && rect.height > 12 && rect.width > 60)
        {
            Mat small = bw_blue_edge(rect);
            plateImg = src(rect);
            candidates_img.push_back(plateImg);
            imshow("plateImage", plateImg);
            waitKey(0);
        }
    }
    return candidates_img;
}

int main()
{
    Mat srcImg = imread("../../imgdata/carID1.jpg");

    if (!srcImg.data)
        return 1;

    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_RGB2GRAY);
    imshow("srcImage", srcImg);
    // sobel 提取边缘
    Mat sobelMat;
    SobelVerEdge(srcGray, sobelMat);
    imshow("Sobel", sobelMat);
    // 疑似区域提取
    vector<Mat> result = getPlateArea(srcImg, sobelMat);
    return 0;
}
