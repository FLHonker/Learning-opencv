// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-5_ORB.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: ORB:An efficient alternative to SIFT or SURF
# @Create Time: 2018-10-25 20:53:26
# @Last Modified: 2018-10-25 20:53:26
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>

using namespace cv;
using namespace std;

// 计算图像ORB特征及匹配
Mat cacORBFeatureAndCompare(Mat src1, Mat src2)
{
    CV_Assert(!src1.empty() && !src2.empty());
    // 关键点检测
    vector<KeyPoint> keypoint1, keypoint2;
    Ptr<ORB> orb = ORB::create();    // 不同于OpenCV3.1
    orb->detect(src1, keypoint1);
    orb->detect(src2, keypoint2);
    // 计算特征向量
    Mat descriptorMat1, descriptorMat2;
    orb->compute(src1, keypoint1, descriptorMat1);
    orb->compute(src2, keypoint2, descriptorMat2);
    // 特征匹配
    BFMatcher matcher(NORM_HAMMING);
    vector<DMatch> matches;
    matcher.match(descriptorMat1, descriptorMat2, matches);
    // 绘制匹配点集
    Mat matchMat;
    drawMatches(src1, keypoint1, src2, keypoint2, matches, matchMat);
    imshow("matchMat", matchMat);
    return matchMat;
}

int main()
{
    Mat srcImg1 = imread("../../imgdata/hand1.jpg", 1);
    Mat srcImg2 = imread("../../imgdata/hand3.jpg", 1);
    if(srcImg1.empty() || srcImg2.empty())
        return -1;
    // ORB特征检测与匹配
    Mat matchMat = cacORBFeatureAndCompare(srcImg1, srcImg2);
    imshow("matchMat", matchMat);
    waitKey(0);
    return 0;
}
