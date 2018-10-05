// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-12_medianBlur.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 中值滤波
# @Create Time: 2018-10-05 16:34:21
# @Last Modified: 2018-10-05 16:34:21
***********************************************/
#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// 中值滤波实现
void myMedianBlur(Mat& src, Mat& dst, const int kSize)
{
    // 目标图像
    dst = src.clone();
    vector<uchar> vList;
    const int nPix = (kSize*2+1) * (kSize*2+1);
    // 遍历图像源
    for(int i = kSize; i < dst.rows - kSize; ++i)
    {
        for(int j = kSize; j < dst.cols - kSize; ++j)
        {
            // 滑窗进行搜索遍历
            for(int pi = i-kSize; pi <= i+kSize; ++pi)
            {
                for(int pj = j-kSize; pj <= j+kSize; ++pj)
                {
                    // 将窗内的元素放到向量内
                    vList.push_back(src.at<uchar>(pi,pj));
                }
            }
            // 窗内元素排序
            sort(vList.begin(), vList.end());
            // 提取中间元素作为当前元素
            dst.at<uchar>(i,j) = vList[nPix/2];
            // 清除当前窗内元素
            vList.clear();
        }
    }
}

// test
int main()
{
    Mat srcImg = imread("../../imgdata/building2.jpg", 0);
    if(srcImg.empty())
        return -1;
    Mat dstImg;
    const int ksize = 2;
    myMedianBlur(srcImg, dstImg, ksize);
    imshow("srcImage", srcImg);
    imshow("medianBlurImage", dstImg);
    waitKey(0);
    return 0;
}
