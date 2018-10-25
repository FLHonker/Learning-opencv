// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-3_SIFT.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: SIFT特征检测及匹配
# @Create Time: 2018-10-24 21:39:54
# @Last Modified: 2018-10-24 21:39:54
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <vector>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

// 计算图像的SIFT特征及匹配
Mat cacSIFTFeatureAndCompare(Mat src1, Mat src2)
{
    // 判断输入的合法性
    CV_Assert(!src1.empty() && !src2.empty());
    // 转换为灰度图
    Mat grayMat1, grayMat2;
    cvtColor(src1, grayMat1, CV_BGR2GRAY);
    cvtColor(src2, grayMat2, CV_BGR2GRAY);
    // 归一化处理
    normalize(grayMat1, grayMat1, 0, 255, NORM_MINMAX);
    normalize(grayMat2, grayMat2, 0, 255, NORM_MINMAX);
    // 定义SIFT描述子
    SiftFeatureDetector detector;
    SiftDescriptorExtractor extractor;
    // 特征点检测
    vector<KeyPoint> keypoints1, keypoints2;
    detector.detect(grayMat1, keypoints1);
    detector.detect(grayMat2, keypoints2);
    // 计算特征点的描述子
    Mat descriptors1, descriptors2;
    extractor.compute(grayMat1, keypoints1, descriptors1);
    extractor.compute(grayMat2, keypoints2, descriptors2);
    // 特征点匹配
    vector<DMatch> matches;
    BFMatcher bfmatcher(NORM_HAMMING);
    bfmatcher.match(descriptors1, descriptors2, matches);
    // 二分排序
    int N = 80;
    nth_element(matches.begin(), matches.begin()+N-1, matches.end());
    // 去除特征点不匹配的情况
    matches.erase(matches.begin()+N, matches.end());
    // 绘制检测结果
    Mat matchMat;
    drawMatches(src1, keypoints1, src2, keypoints2, matches, matchMat);
    // imshow("matchMat", matchMat);
    return matchMat;
}

int main()
{
    Mat srcImg1 = imread("../../imgdata/hand1.jpg", 1);
    Mat srcImg2 = imread("../../imgdata/hand2.jpg", 1);
    if(srcImg1.empty() || srcImg2.empty())
        return -1;
    // SIFT特征描述
    Mat resSiftMatchMat = cacSIFTFeatureAndCompare(srcImg1, srcImg2);
    imshow("resSiftMatchMat", resSiftMatchMat);
    waitKey(0);
    return 0;
}

