// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-15_MSER.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: MSER候选车牌区域检测
# @Create Time: 2018-10-31 11:30:45
# @Last Modified: 2018-10-31 11:30:45
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

// MSER车牌目标检测
vector<Rect> mserGetPlate(Mat src)
{
    // HSV空间转换
    Mat hsv, gray, gray_neg;
    cvtColor(src, hsv, CV_BGR2HSV);
    // 通道分离
    vector<Mat> channels;
    split(hsv, channels);
    // 提取H通道
    gray = channels[1];
    // 灰度转换
    cvtColor(src, gray, CV_BGR2GRAY);
    // 取反值灰度
    gray_neg = 255 - gray;
    vector<vector<Point>> regContours, charContours;
    // 创建MSER对象
    Ptr<MSER> mser_plus = MSER::create(2, 10, 5000, 0.5, 0.3);
    Ptr<MSER> mser_sub = MSER::create(2, 2, 400, 0.1, 0.3);
    vector<Rect> bboxes1, bboxes2;
    // MSER+操作
    mser_plus->detectRegions(gray, regContours, bboxes1);
    // MSER-操作
    mser_sub->detectRegions(gray_neg, charContours, bboxes2);
    Mat mserMapMat = Mat::zeros(src.size(), CV_8UC1);
    Mat mserNegMapMat = Mat::zeros(src.size(), CV_8UC1);
    // MSER+检测
    for(int i = regContours.size()-1; i >= 0; --i)
    {
        // 根据检测区域点生成MSER+结果
        const vector<Point> &r = regContours[i];
        for(int j = 0; j < r.size(); ++j)
        {
            Point pt = r[j];
            mserMapMat.at<uchar>(pt) = 255;
        }
    }
    // MSER-检测
    for(int i = charContours.size()-1; i >= 0; --i)
    {
        // 根据检测区域点生成MSER-结果
        const vector<Point> &r = charContours[i];
        for(int j = 0; j < r.size(); ++j)
        {
            Point pt = r[j];
            mserNegMapMat.at<uchar>(pt) = 255;
        }
    }
    // MSER结果输出
    Mat mserResMat;
    // MSER+与MSER-位与操作
    mserResMat = mserMapMat & mserNegMapMat;
    imshow("mserMapMat", mserMapMat);
    imshow("mserNegMapMat", mserNegMapMat);
    imshow("mserResMat", mserResMat);
    // 闭操作连接缝隙
    Mat mserClosedMat;
    morphologyEx(mserResMat, mserClosedMat, MORPH_CLOSE, Mat::ones(1, 20, CV_8UC1));
    imshow("mserClosedMat", mserClosedMat);
    // 寻找外部轮廓
    vector<vector<Point>> plateContours;
    findContours(mserClosedMat, plateContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 候选车牌区域判断输出
    vector<Rect> candidates;
    for(int i = 0; i < plateContours.size(); ++i)
    {
        // 求解最小外界矩形
        Rect rect = boundingRect(plateContours[i]);
        // 宽高比例
        double wh_ratio = rect.width / (double)rect.height;
        // 符合条件尺寸判断
        if(rect.height > 20 && wh_ratio > 4 && wh_ratio < 7)
            candidates.push_back(rect);
    }
    return candidates;
}

int main()
{
    Mat srcImg = imread("../../imgdata/carID.jpg");
    if(srcImg.empty())
        return -1;
    // MSER候选车牌区域检测
    vector<Rect> candidates = mserGetPlate(srcImg);
    // 车牌区域显示
    for(int i = 0; i < candidates.size(); ++i)
    {
        imshow("plate", srcImg(candidates[i]));
        waitKey(0);
    }
    return 0;
}
