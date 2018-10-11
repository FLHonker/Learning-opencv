// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-20_convexHull.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 凸包及缺陷检测
# @Create Time: 2018-10-11 12:55:49
# @Last Modified: 2018-10-11 12:55:49
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

// 设置全局参数
Mat srcImg, srcGray;
int thresh = 100, max_thresh = 255;
RNG rng(12345);

// mouse callback event
void thresh_callback(int, void*)
{
    Mat srcTmp = srcImg.clone();
    Mat threMat;
    // 轮廓检测参数
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // 阈值化操作
    threshold(srcGray, threMat, thresh, 255, THRESH_BINARY);
    // 轮廓检测
    findContours(threMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    // 凸包及缺陷检测参数
    vector< vector<Point> > pointHull(contours.size());
    vector< vector<int> > intHull(contours.size());
    vector< vector<Vec4i> > hullDefect(contours.size());
    for(size_t i = 0; i < contours.size(); ++i)
    {
        // Point 类型凸包检测
        convexHull(Mat(contours[i]), pointHull[i], false);
        // int 类型凸包检测
        convexHull(Mat(contours[i]), intHull[i], false);
        // 凸包缺陷检测
        convexityDefects(Mat(contours[i]), intHull[i], hullDefect[i]);
    }
    // 绘制凸包及缺陷检测结果
    Mat drawing = Mat::zeros(threMat.size(), CV_8UC3);
    for(size_t i = 0; i < contours.size(); ++i)
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        drawContours(drawing, pointHull , i, color, 1, 8, vector<Vec4i>(), 0, Point());
        // 绘制缺陷
        size_t count = contours[i].size();
        if(count > 300)
            continue;
        // 设置凸包缺陷迭代器
        vector<Vec4i>::iterator iterDefects = hullDefect[i].begin();
        // 遍历得到4个特征量
        while(iterDefects != hullDefect[i].end())
        {
            Vec4i& v = *iterDefects;
            // 起始位置
            int startidx = v[0];
            Point ptStart(contours[i][startidx]);
            // 终止位置
            int endidx = v[1];
            Point ptEnd(contours[i][endidx]);
            // 内凸壳的最远点缺陷
            int faridx = v[2];
            Point ptFar(contours[i][faridx]);
            // 凸包之间的最远点
            int depth = v[3] / 256;
            // 绘制相应的线与圆检测结果
            if(depth > 20 && depth < 80)
            {
                line(drawing, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
                line(drawing, ptEnd, ptFar, CV_RGB(0, 255, 0), 2);
                circle(drawing, ptStart, 4, Scalar(255, 0, 100), 2);
                circle(drawing, ptEnd, 4, Scalar(255, 0, 100), 2);
                circle(drawing, ptFar, 4, Scalar(100, 0, 255), 2);
            }
            iterDefects++;
        }
    }
    imshow("drawing", drawing);
}

int main()
{
    srcImg = imread("../../imgdata/smarties.png");
    if(srcImg.empty())
        return -1;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    namedWindow("srcImage", CV_WINDOW_AUTOSIZE);
    imshow("srcImage", srcImg);
    // 调用鼠标操作事件
    createTrackbar(" Threshold:", "srcImage", &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);
    waitKey(0);
    return 0;
}
