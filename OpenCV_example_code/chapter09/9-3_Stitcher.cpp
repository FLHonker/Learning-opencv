// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-3_Stitcher.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 图像拼接
# @Create Time: 2018-11-18 14:11:25
# @Last Modified: 2018-11-18 14:11:25
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

// OpenCV stitcher类操作
void stitch(vector<Mat> imgs, Mat& resultMat)
{
    bool try_use_gpu = true;
    // 定义Stitcher类
    Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
    // 调用stitch方法完成拼接
    Stitcher::Status status = stitcher.stitch(imgs, resultMat);
    if(status != Stitcher::OK)
        cout << "stitch error" << endl;
}

// 基于特征点匹配的图像拼接
void stitch2(Mat& src1, Mat& src2, Mat& panorama)
{
    // SURF特征点描述
    Mat srcImg2Warped;
    int minHessian = 400;
    Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(minHessian);
    vector<KeyPoint> kPointMat1, kPointMat2;
    detector->detect(src1, kPointMat1);
    detector->detect(src2, kPointMat2);
    // 计算特征向量
    Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();
    Mat desMat1, desMat2;
    extractor->compute(src1, kPointMat1, desMat1);
    extractor->compute(src2, kPointMat2, desMat2);
    // FLANN关键点匹配
    // FLANN初始化，RANSAC一致性，最近邻特征
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(desMat1, desMat2, matches);
    double max_dist = 0, min_dist = 100;
    // 特征点最大与最小距离查找
    for(int i = 0; i < matches.size(); ++i)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }
    // 使用good特征构建距离限定
    vector<DMatch> good_matches;
    for(int i = 0; i < matches.size(); ++i)
    {
        // 特征点最优距离筛选
        if(matches[i].distance < 3*min_dist)
            good_matches.push_back(matches[i]);
    }
    // 图像的关键点匹配
    vector<Point2f> src1_matchedKPs, src2_matchedKPs;
    for(size_t i = 0; i < good_matches.size(); ++i)
    {
        src1_matchedKPs.push_back(kPointMat1[good_matches[i].queryIdx].pt);
        src2_matchedKPs.push_back(kPointMat2[good_matches[i].trainIdx].pt);
    }
    // 计算src1与src2的映射
    Mat H = findHomography(Mat(src2_matchedKPs), Mat(src1_matchedKPs), CV_RANSAC);
    // 仿射变换
    warpPerspective(src2, srcImg2Warped, H, Size(src2.cols*2, src2.rows), INTER_CUBIC);
    panorama = srcImg2Warped.clone();
    // 结果输出
    Mat roi(panorama, Rect(0, 0, src1.cols, src1.rows));
    src1.copyTo(roi);
}

int main()
{
    Mat image1 = imread("../../imgdata/img07.jpg");
    Mat image2 = imread("../../imgdata/img08.jpg");
    Mat image3 = imread("../../imgdata/img09.jpg");
    if(image1.empty() || image2.empty() || image3.empty())
        return -1;
    vector<Mat> imgs;
    imgs.push_back(image1);
    imgs.push_back(image2);
    imgs.push_back(image3);
    Mat resultMat1, resultMat2;
    stitch(imgs, resultMat1);
    imshow("stitch1", resultMat1);
    stitch2(image1, image2, resultMat2);
    //stitch2(resultMat2, image3, resultMat2);
    imshow("stitch2", resultMat2);
    waitKey(0);
    return 0;
}

