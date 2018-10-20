// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-7_blobDetect.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 奇异区域检测
# @Create Time: 2018-10-20 10:09:30
# @Last Modified: 2018-10-20 10:09:30
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
using namespace cv;
using namespace std;

int main()
{
    Mat srcImg= imread("../../imgdata/shapes3.jpg");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    // 向量关键点
    vector<KeyPoint> keypoints;
    // 定义blob类
    SimpleBlobDetector::Params params;
    // 参数定义
    params.filterByArea = true;
    params.minArea = 15;
    params.maxArea = 10000;
    SimpleBlobDetector blobDetector(params);
    blobDetector.create("SimpleBlob");
    // 奇异区域检测
    blobDetector.detect(srcImg, keypoints);
    // 绘制关键点
    drawKeypoints(srcImg, keypoints, srcImg, Scalar(255,0,0));
    imshow("keypoints", srcImg);
    waitKey(0);
    return 0;
}
