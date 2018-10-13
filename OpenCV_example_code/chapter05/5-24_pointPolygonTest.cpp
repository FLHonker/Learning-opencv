// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-24_pointPolygonTest.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 点多边形测试
# @Create Time: 2018-10-12 14:12:55
# @Last Modified: 2018-10-12 14:12:55
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

vector< vector<Point> > closed_contours;
vector<Vec4i> hierarchy;
Mat contoursAllMat;

// 函数多边形近似
vector<vector<Point>> cacContoursClosed(vector<vector<Point>> contours)
{
    vector<vector<Point>> vContours;
    vContours.resize(contours.size());
    for(int i = 0; i < vContours.size(); ++i)
        approxPolyDP(contours[i], vContours[i], 0.1, true);
    return vContours;
}

// 最小点集边界轮廓
int cacContoursSmall(Point p, vector<vector<Point>> contours, vector<Vec4i> hierarchy)
{
    int idx = 0, prev_idx = -1;
    while(idx >= 0)
    {
        vector<Point> c = contours[idx];
        // 点多边形测试
        double d = pointPolygonTest(c, p, false);
        // 判断点集是否在多边形内部
        if(d > 0)
        {
            prev_idx = idx;
            idx = hierarchy[idx][2];
        } else {
            // 不在则继续遍历下一个
            idx = hierarchy[idx][0];
        }
    }
    return prev_idx;
}

// 鼠标点击动作响应
void onMouse(int event, int x, int y, int, void*)
{
    // 事件左键确认
    if(event != EVENT_LBUTTONDOWN)
        return;
    Point p(x, y);
    // 寻找最小封闭外界轮廓
    int idxContour = cacContoursSmall(p, closed_contours, hierarchy);
    // 如果不存在轮廓，显示将返回
    if(idxContour < 0)
    {
        imshow("Contours", contoursAllMat);
        return;
    }
    // 绘制所有最小的外界轮廓
    vector<vector<Point>> viewContour;
    viewContour.push_back(closed_contours[idxContour]);
    if(!viewContour.empty())
    {
        Mat result = contoursAllMat.clone();
        drawContours(result, viewContour, -1, Scalar(0,0,255), 3);
        imshow("Contours", result);
    }
}

int main()
{
    Mat srcImg = imread("../../imgdata/shapes1.jpg");
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    contoursAllMat = srcImg.clone();
    Mat srcGray, edges;
    // 边缘检测
    cvtColor(srcImg, srcGray, CV_RGB2GRAY);
    Canny(srcGray, edges, 50, 100);
    // 寻找外轮廓
    vector<vector<Point>> contours;
    findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    // 确认检测轮廓点的封闭性
    closed_contours = cacContoursClosed(contours);
    // 绘制所有轮廓
    drawContours(srcImg, closed_contours, -1, Scalar(0,255,0));
    imshow("Contours", srcImg);
    // 鼠标回调事件
    setMouseCallback("Contours", onMouse);
    while(char(waitKey(1)) != 'q') {};
    return 0;
}
