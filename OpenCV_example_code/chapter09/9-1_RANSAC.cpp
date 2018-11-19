// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-1_RANSAC.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 特征运动的估计，RANSAC随机一致性算法
# @Create Time: 2018-11-02 13:23:20
# @Last Modified: 2018-11-02 13:23:20
***********************************************/
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

// good特征点鉴别
Mat getRansacMat(const vector<DMatch>& matches,
                 const vector<KeyPoint>& keypoints1,
                 const vector<KeyPoint>& keypoints2,
                 vector<DMatch>& outMatches)
{
    // 转换特征点格式
    vector<Point2f> points1, points2;
    Mat featureMat;
    for(vector<DMatch>::const_iterator it = matches.begin(); it != matches.end(); ++it)
    {
        // 获取特征点的左侧位置
        float x = keypoints1[it->queryIdx].pt.x;
        float y = keypoints1[it->queryIdx].pt.y;
        points1.push_back(Point2f(x,y));
        // 获取特征点的左侧位置
        x = keypoints2[it->queryIdx].pt.x;
        y = keypoints2[it->queryIdx].pt.y;
        points2.push_back(Point2f(x,y));
    }
    // 计算good特征矩阵
    vector<uchar> inliers(points1.size(), 0);
    if(points1.size() > 0 && points2.size() > 0)
    {
        // 计算两幅图像的对应点的特征矩阵
        Mat featureMat = findFundamentalMat(Mat(points1), Mat(points2), inliers, CV_FM_RANSAC, distance, confidence);
        // 提取存在特征点匹配
        vector<uchar>::const_iterator itIn = inliers.begin();
        vector<DMatch>::const_iterator itM = matches.begin();
        // 遍历所有特征点
        for( ; itIn != inliers.end(); ++itIn, ++itM)
        {
            if(*itIn)
                outMatches.push_back(*itM);
        }
        if(refineF)
        {
            points1.clear();
            points2.clear();
            for(vector<DMatch>::const_iterator it = outMatches.begin(); it != outMatches.end(); ++it)
            {
                float x = keypoints1[it->queryIdx].pt.x;
                float y = keypoints1[it->queryIdx].pt.y;
                points1.push_back(Point2f(x,y));
                x = keypoints2[it->queryIdx].pt.x;
                y = keypoints2[it->queryIdx].pt.y;
                points2.push_back(Point2f(x,y));
            }
            // 计算两幅图像的特征矩阵
            if(points1.size() > 0 && points2.size() > 0)
            {
                featureMat = findFundamentalMat(Mat(points1), Mat(points2), CV_FM_8POINT);
            }
        }
    }
    return featureMat;
}
