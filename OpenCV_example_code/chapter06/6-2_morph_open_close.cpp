// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 6-2_morph_open_close.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 形态学开闭操作
# @Create Time: 2018-10-15 15:40:11
# @Last Modified: 2018-10-15 15:40:11
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat srcImg = imread("../../imgdata/test.jpg");
    if(srcImg.empty())
        return -1;
    Mat srcGray, segImg;
    cvtColor(srcImg, srcGray, CV_BGR2GRAY);
    // 阈值化
    threshold(srcGray, segImg, 255 * 0.5, 255, THRESH_BINARY);
    // 定义结构元素
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(20, 20));
    // 形态学闭操作
    Mat closedMat;
    morphologyEx(segImg, closedMat, MORPH_CLOSE, element);
    // 形态学开操作
    Mat openedMat;
    morphologyEx(segImg, openedMat, MORPH_OPEN, element);
    imshow("srcImage", srcImg);
    imshow("segImage", segImg);
    imshow("closedMat", closedMat);
    imshow("openedMat", openedMat);
    waitKey(0);
    return 0;
}
