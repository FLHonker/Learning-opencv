// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-4_SURF.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: SURF:Speed Up Robust Features
# @Create Time: 2018-10-25 20:27:18
# @Last Modified: 2018-10-25 20:27:18
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

// 计算图像的SURF特征及匹配
float cacSURFFeatureAndCompare(Mat src1, Mat src2, float param)
{
    CV_Assert(!src1.empty() && !src2.empty());
    // 转为灰度
    Mat grayMat1, grayMat2;
    cvtColor(src1, grayMat1, CV_BGR2GRAY);
    cvtColor(src2, grayMat2, CV_BGR2GRAY);
    // 初始化SURF检测描述子
    Ptr<SurfFeatureDetector> surfDetector = SurfFeatureDetector::create(param);
    Ptr<SurfDescriptorExtractor> surfExtractor = SurfDescriptorExtractor::create();
    // 关键点及特征描述矩阵声明
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptorMat1, descriptorMat2;
    // 计算SURF特征关键点
    surfDetector->detect(grayMat1, keypoints1);
    surfDetector->detect(grayMat2, keypoints2);
    // 计算SURF特征描述矩阵
    surfExtractor->compute(grayMat1, keypoints1, descriptorMat1);
    surfExtractor->compute(grayMat2, keypoints2, descriptorMat2);
    float result = 0;
    // 特征点匹配
    if(keypoints1.size() > 0 && keypoints2.size() > 0)
    {
        // 计算特征匹配点
        FlannBasedMatcher matcher;
        // 特征点匹配向量
        vector<DMatch> matches;
        // 符合距离特征点匹配向量
        vector<DMatch> viewMatches;
        matcher.match(descriptorMat1, descriptorMat2, matches);
        // 判断最优匹配
        double minDist = 100;
        for(int i = 0; i < matches.size(); ++i){
            if(matches[i].distance < minDist)
                minDist = matches[i].distance;
        }
        // 计算距离特征符合要求的特征点
        int num = 0;
        cout << "minDist:" << minDist <<endl;
        for(int i = 0; i < matches.size(); ++i)
        {
            // 判断特征点匹配距离
            if(matches[i].distance <= 2*minDist)
            {
                result += matches[i].distance * matches[i].distance;
                // 计算符合距离特征的下特征点
                viewMatches.push_back(matches[i]);
                num++;
            }
        }
        // 计算匹配度
        result /= num;
        // 绘制特征点匹配结果
        Mat matchMat;
        drawMatches(src1, keypoints1, src2, keypoints2, matches, matchMat);
        imshow("matchMat", matchMat);
        waitKey(0);
    }
    return result;
}

int main()
{
    Mat srcImg1 = imread("../../imgdata/hand1.jpg");
    Mat srcImg2 = imread("../../imgdata/hand3.jpg");
    if(srcImg1.empty() || srcImg2.empty())
        return -1;
    // 计算SURF特征及匹配
    float matchRate = cacSURFFeatureAndCompare(srcImg1, srcImg2, 1000);
    cout << "matchRate:" << matchRate <<endl;
    return 0;
}
