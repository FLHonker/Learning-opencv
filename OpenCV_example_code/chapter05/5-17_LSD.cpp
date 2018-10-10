// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-17_LSD.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: LSD(LineSegmentDetector)快速线检测
# @Create Time: 2018-10-10 16:57:58
# @Last Modified: 2018-10-10 16:57:58
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(srcImg.empty())
        return -1;
    imshow("srcImage", srcImg);
    // Canny边缘检测
    Canny(srcImg, srcImg, 50, 200, 3);
    // 创建LSD检测类
#if 1
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_STD);
#else 
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(LSD_REFINE_NONE);
#endif
    double start = double(getTickCount());
    vector<Vec4f> vecLines;
    // 线检测
    lsd->detect(srcImg, vecLines);
    double times = ((double)getTickCount() - start) * 1000 / getTickFrequency();
    cout << "time-cost: " << times << " ms" <<endl;
    // 绘图线检测结果
    Mat linesMat(srcImg);
    lsd->drawSegments(linesMat, vecLines);
    imshow("linesImage", linesMat);
    waitKey(0);
    return 0;
}
