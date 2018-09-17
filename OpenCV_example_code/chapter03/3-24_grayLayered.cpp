// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-24_grayLayered.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 灰度级分层
# 更好地提取某些区域灰度的亮度特征，修复某些场景下的图像质量缺陷。
# 两种方法：
# (1) 将待提取的感兴趣区域的灰度值映射为同一值（最大值），其他不感兴趣的
#     区域映射为另一值（最小值），最终输出图像为二值图像。
# (2) 将待提取的感兴趣区域的灰度值映射变大或变小，其他不变，最终输出仍为灰度图。
# @Create Time: 2018-09-17 15:18:23
# @Last Modified: 2018-09-17 15:18:23
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

/*
 * @input: Mat srcGray, 输入灰度图
 * @return: Mat resultImg 
 */
Mat grayLayered(Mat srcGray)
{
    Mat resultImg = srcGray.clone();
    int nRows = resultImg.rows, nCols = resultImg.cols;
    // 图像连续性判断
    if(resultImg.isContinuous())
    {
        nCols = nCols * nRows;
        nRows = 1;
    }
    // 图像指针操作
    uchar* pDataMat;
    int pixMin = 150, pixMax = 200;
    // 计算图像的灰度级分层
    for(int i = 0; i < nRows; ++i)
    {
        pDataMat = resultImg.ptr<uchar>(i);
        for(int j = 0; j < nCols; ++j)
        {
            // 方法1：二值映射
            if(pDataMat[j] > pixMin)
                pDataMat[j] = 255;
            else
                pDataMat[j] = 0;
            // 方法2：灰度映射
            // if(pDataMat[j] > pixMin && pDataMat[j] < pixMax)
            //      pDataMat[j] = pixMax；
        }
    }
    return resultImg;
}

int main()
{
    Mat srcImg = imread("../../imgdata/panada.jpg");
    if(!srcImg.data)
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    Mat resultImg = grayLayered(srcGray);
    imshow("resultImage", resultImg);
    waitKey(0);
    return 0;
}
