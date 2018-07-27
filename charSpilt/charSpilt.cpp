// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: charSpilt.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: ---
# @Create Time: 2018-07-24 10:04:03
# @Last Modified: 2018-07-24 10:04:03
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "Usage: ./charSpilt img" << endl;
        return -1;
    }
    Mat imgSrc = imread(argv[1], 1);
    imshow("sourceImg", imgSrc);
    Mat img_gray;
    cvtColor(imgSrc, img_gray, CV_BGR2GRAY);
    threshold(img_gray, img_gray, 100, 255, CV_THRESH_BINARY_INV);
    imshow("thresholdImg", img_gray);
    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img_gray, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    Mat img1;
    Mat img2;
    Mat img3;
    Mat img4;
    Mat img5;


    for(int i=0; i<contours.size();i++)
    {
        Rect rect = boundingRect(Mat(contours[i]));
        rectangle(imgSrc, rect.tl(), rect.br(),Scalar(255,0,0));
        Mat roi = imgSrc(rect);
        switch(i)
        {
        case 1:
            roi.convertTo(img1, roi.type());
        case 4:
            roi.convertTo(img2, roi.type());
        case 5:
            roi.convertTo(img3, roi.type());
        case 7:
            roi.convertTo(img4, roi.type());
        case 8:
            roi.convertTo(img5, roi.type());

        }
    }
    imshow("img1",img1);
    imshow("img2",img2);
    imshow("img3",img3);
    imshow("img4",img4);
    imshow("img5",img5);
    imshow("contoursImg", imgSrc);

    waitKey(0);
    return 0;
}
