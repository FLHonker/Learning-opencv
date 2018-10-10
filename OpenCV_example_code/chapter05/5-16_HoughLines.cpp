// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-16_HoughLines.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 线检测——霍夫变换
# @Create Time: 2018-10-10 12:10:05
# @Last Modified: 2018-10-10 12:10:05
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
        return -1;
    Mat edgeMat, houghMat;
    // Canny 边缘检测，二值图像
    Canny(srcImg, edgeMat, 50, 200, 3);
    cvtColor(edgeMat, houghMat, CV_GRAY2BGR);
#if 0
    // 标准的霍夫变换
    vector<Vec2f> lines;
    HoughLines(edgeMat, lines, 1, CV_PI/180, 100, 0, 0);
    for(size_t i = 0; i < lines.size(); ++i)
    {
        // 根据直线参数表达式绘制相应的检测结果
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*a);
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*a);
        line(houghMat, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
    }
#else
    // 统计概率的霍夫变换
    vector<Vec4i> lines;
    HoughLinesP(edgeMat, lines, 1, CV_PI/180, 50, 50, 10);
    for(size_t i = 0; i < lines.size(); ++i)
    {
        Vec4i li = lines[i];
        // 绘制线检测结果
        line(houghMat, Point(li[0], li[1]), Point(li[2], li[3]), Scalar(0, 255, 0), 3, CV_AA);
    }
#endif
    imshow("srcImage", srcImg);
    imshow("houghLines", houghMat);
    waitKey(0);
    return 0;
}
