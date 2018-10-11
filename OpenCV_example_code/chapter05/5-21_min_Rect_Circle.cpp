// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-21_min_Rect_Circle.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 最小矩形与圆生成
# @Create Time: 2018-10-11 15:23:05
# @Last Modified: 2018-10-11 15:23:05
***********************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
    Mat img(500, 500, CV_8UC3);
    // 随机生成因子
    RNG rng = theRNG();
    for(;;)
    {
        int i, count = rng.uniform(1, 101);
        vector<Point> points;
        // 随机生成二维点集
        for(i = 0; i < count; ++i)
        {
            Point pt;
            pt.x = rng.uniform(img.cols / 4, img.cols * 3/4);
            pt.y = rng.uniform(img.rows / 4, img.rows * 3/4);
            points.push_back(pt);
        }
        // 计算二维点集形成的矩形
        RotatedRect box = minAreaRect(points);
        Point2f center, vtx[4];
        float radius = 0;
        minEnclosingCircle(Mat(points), center, radius);
        box.points(vtx);
        img = Scalar::all(0);
        // 绘制二维点集形成的圆
        for(i = 0; i < count; ++i)
            circle(img, points[i], 3, Scalar(0,0,255), CV_FILLED, CV_AA);
        circle(img, center, cvRound(radius), Scalar(0,255,255), 1, CV_AA);
        // 绘制二维点集形成的矩形
        for(i = 0; i < 4; ++i)
            line(img, vtx[i], vtx[(i+1)%4], Scalar(0,255,0), 1, CV_AA);
        imshow("rect & circle", img);
        char key = (char)cvWaitKey();
        if(key == 27 || key == 'q' || key == 'Q')   // 'ESC'
            break;
    }
    return 0;
}
