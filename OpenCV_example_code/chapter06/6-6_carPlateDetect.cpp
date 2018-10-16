// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 6-6_carPlateDetect.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 车牌区域提取
# @Create Time: 2018-10-16 11:10:58
# @Last Modified: 2018-10-16 11:10:58
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat getPlate(int width, int height, Mat srcGray)
{
    Mat dst;
    // 形态学梯度检测边缘
    morphologyEx(srcGray, dst, MORPH_GRADIENT, Mat(1, 2, CV_8U, Scalar(1)));
    imshow("result1", dst);
    // 阈值化
    threshold(dst, dst, 255 * 0.1, 255, THRESH_BINARY);
    imshow("result2", dst);
    // 水平方向闭运算
    int nCols = 0;
    if(width >= 400 && width < 600)
        nCols = 25;
    else if(width >= 200 && width < 300)
        nCols = 20;
    else if(width >= 600)
        nCols = 28;
    else 
        nCols = 15;
    morphologyEx(dst, dst, MORPH_CLOSE, Mat(1, nCols, CV_8U, Scalar(1)));
    // 垂直方向闭运算
    int nRows = 0;
    if(height >= 400 && height < 600)
        nRows = 8;
    else if(height >= 200 && height < 300)
        nRows = 6;
    else if(height >= 600)
        nRows = 10;
    else
        nRows = 4;
    morphologyEx(dst, dst, MORPH_CLOSE, Mat(nRows, 1, CV_8U, Scalar(1)));
    return dst;
}

int main()
{
    Mat srcImg = imread("../../imgdata/carID1.jpg");
    if(srcImg.empty())
        return -1;
    Mat srcGray;
    cvtColor(srcImg, srcGray, CV_RGB2GRAY);
    imshow("srcGray", srcGray);
    Mat result = getPlate(400, 300, srcGray);
    // 连通域检测
    vector<vector<Point>> blue_contours;
    vector<Rect> blue_rect;
    findContours(result.clone(), blue_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 连通域遍历，车牌目标提取
    for(size_t i = 0; i < blue_contours.size(); ++i)
    {
        Rect rect = boundingRect(blue_contours[i]);
        double wh_ratio = (double)rect.width / rect.height;
        int sub = countNonZero(result(rect));
        double ratio = (double)sub / rect.area();
        if(wh_ratio > 2 && wh_ratio < 8 && rect.height > 12 && rect.width > 60 && ratio > 0.4)
        {
            blue_rect.push_back(rect);
            imshow("plate", srcImg(rect));
            waitKey(0);
        }
    }
    imshow("result3", result);
    waitKey(0);
    return 0;
}
