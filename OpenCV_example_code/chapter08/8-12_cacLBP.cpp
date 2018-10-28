// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-12_cacLBP.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 计算LBP特征
# @Create Time: 2018-10-28 13:30:17
# @Last Modified: 2018-10-28 13:30:17
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;

// 计算原始LBP特征
Mat OLBP(Mat& src)
{
    const int nRows = src.rows;
    const int nCols = src.cols;
    Mat dst(src.size(), src.type());
    // 遍历图像，生成LBP特征
    for(int y = 1; y < nRows - 1; ++y)
    {
        for(int x = 1; x < nCols - 1; ++x)
        {
            // 定义邻域
            uchar neighbor[8] = { 0 };
            neighbor[0] = src.at<uchar>(y-1, x-1);
            neighbor[1] = src.at<uchar>(y-1, x);
            neighbor[2] = src.at<uchar>(y-1, x+1);
            neighbor[3] = src.at<uchar>(y, x+1);
            neighbor[4] = src.at<uchar>(y+1, x+1);
            neighbor[5] = src.at<uchar>(y+1, x);
            neighbor[6] = src.at<uchar>(y+1, x-1);
            neighbor[7] = src.at<uchar>(y, x-1);
            // 当前图像的处理中心
            uchar center = src.at<uchar>(y, x);
            uchar code = 0;
            // 计算LBP的值
            for(int k = 0; k < 8; ++k)
            {
                // 遍历中心点邻域并比较
                code += (neighbor[k] >= center) * (1 << k);
            }
            dst.at<uchar>(y, x) = code;
        }
    }
    return dst;
}

int main()
{
    Mat srcImg = imread("../../imgdata/hand1.jpg", 0);
    if(srcImg.empty())
        return -1;
    Mat dstImg = OLBP(srcImg);
    imshow("srcImage", srcImg);
    imshow("OLBP", dstImg);
    waitKey(0);
    return 0;
}
