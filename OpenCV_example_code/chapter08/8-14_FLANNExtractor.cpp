// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-14_FLANNExtractor.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: FLANN特征点匹配目标提取
# @Create Time: 2018-10-30 12:32:40
# @Last Modified: 2018-10-30 12:32:40
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/flann/miniflann.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

Mat flannMatchExtractObj(Mat src1, Mat src2)
{
    CV_Assert(!src1.empty() && !src2.empty());
    // 构造SURF检测器
    int hessPara = 300;
    Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(hessPara);
    // 特征点检测
    vector<KeyPoint> keypoints1, keypoints2;
    detector->detect(src1, keypoints1);
    detector->detect(src2, keypoints2);
    // 描述子提取
    Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();
    Mat despMat1, despMat2;
    extractor->compute(src1, keypoints1, despMat1);
    extractor->compute(src2, keypoints2, despMat2);
    // FLANN特征点匹配
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(despMat1, despMat2, matches);
    double max_dist = 0, min_dist = 100;
    // 距离判断——最优匹配点
    for(int i = 0; i < despMat1.rows; ++i)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }
    cout << "max_dist:" << max_dist <<endl;
    cout << "min_dist:" << min_dist <<endl;
    // 最佳匹配点
    vector<DMatch> matchVec;
    // 检测点
    for(int i = 0; i < despMat1.rows; ++i)
    {
        if(matches[i].distance < 3*min_dist)
            matchVec.push_back(matches[i]);
    }
    // 绘制检测点
    Mat matchMat;
    drawMatches(src1, keypoints1, src2, keypoints2, matchVec, matchMat, 
                Scalar::all(-1), Scalar::all(-1), vector<char>(), 
                DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow("matchMat", matchMat);
    // 特征点一致性检测
    vector<Point2f> obj, scene;
    for(int i = 0; i < matchVec.size(); ++i)
    {
        obj.push_back(keypoints1[matchVec[i].queryIdx].pt);
        scene.push_back(keypoints2[matchVec[i].trainIdx].pt);
    }
    // 随机一致性
    Mat H = findHomography(obj, scene, CV_RANSAC);
    // 构造变换矩阵
    Point2f objCorner[4] = {
        cvPoint(0, 0), cvPoint(src1.cols, 0), cvPoint(src1.cols, src1.rows), cvPoint(0, src1.rows)
    };
    // 绘制匹配点
    Point sceneCors[4];
    for(int i = 0; i < 4; ++i)
    {
        double x = objCorner[i].x, y = objCorner[i].y;
        // 构造映射矩阵
        double Z = 1.0/(H.at<double>(2,0)*x + H.at<double>(2,1)*y + H.at<double>(2,2));
        double X = (H.at<double>(0,0)*x + H.at<double>(0,1)*y + H.at<double>(0,2)) * Z;
        double Y = (H.at<double>(1,0)*x + H.at<double>(1,1)*y + H.at<double>(1,2)) * Z;
        sceneCors[i] = cvPoint(cvRound(X) + src1.cols, cvRound(Y));
    }
    // 绘制匹配目标
    line(matchMat, sceneCors[0], sceneCors[1], Scalar(0, 255, 0), 2);
    line(matchMat, sceneCors[1], sceneCors[2], Scalar(0, 255, 0), 2);
    line(matchMat, sceneCors[2], sceneCors[3], Scalar(0, 255, 0), 2);
    line(matchMat, sceneCors[3], sceneCors[0], Scalar(0, 255, 0), 2);
    imshow("ObjectMat", matchMat);
    return matchMat;
}

int main()
{
    Mat srcImg1 = imread("../../imgdata/sAcar.jpg");
    Mat srcImg2 = imread("../../imgdata/Acar.jpg");
    if(srcImg1.empty() || srcImg2.empty())
        return -1;
    // Flann特征点匹配目标检测
    Mat resMatchMat = flannMatchExtractObj(srcImg1, srcImg2);
    waitKey(0);
    return 0;
}
