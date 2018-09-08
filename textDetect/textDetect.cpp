// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: textDetect.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: ---
# @Create Time: 2018-07-31 16:07:38
# @Last Modified: 2018-07-31 16:07:38
***********************************************/
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

const string IMAGEPATH = "demo2.jpg";//图像路径
int _tmain(int argc, _TCHAR* argv[])
{
    Mat textImageSrc = imread(IMAGEPATH);
    detect(textImageSrc);

    return 0;
}

Mat preprocess(Mat gray)
{
    //1.Sobel算子，x方向求梯度
    Mat sobel;
    Sobel(gray, sobel, CV_8U, 1, 0, 3);

    //2.二值化
    Mat binary;
    threshold(sobel, binary, 0, 255, THRESH_OTSU + THRESH_BINARY);

    //3.膨胀和腐蚀操作核设定
    Mat element1 = getStructuringElement(MORPH_RECT, Size(30, 9));
    //控制高度设置可以控制上下行的膨胀程度，例如3比4的区分能力更强,但也会造成漏检
    Mat element2 = getStructuringElement(MORPH_RECT, Size(24, 4));

    //4.膨胀一次，让轮廓突出
    Mat dilate1;
    dilate(binary, dilate1, element2);

    //5.腐蚀一次，去掉细节，表格线等。这里去掉的是竖直的线
    Mat erode1;
    erode(dilate1, erode1, element1);

    //6.再次膨胀，让轮廓明显一些
    Mat dilate2;
    dilate(erode1, dilate2, element2);

    //7.存储中间图片
    imwrite("binary.jpg", binary);
    imwrite("dilate1.jpg", dilate1);
    imwrite("erode1.jpg", erode1);
    imwrite("dilate2.jpg", dilate2);

    return dilate2;
}


vector<RotatedRect> findTextRegion(Mat img)
{
    vector<RotatedRect> rects;
    //1.查找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

    //2.筛选那些面积小的
    for (int i = 0; i < contours.size(); i++)
    {
        //计算当前轮廓的面积
        double area = contourArea(contours[i]);

        //面积小于1000的全部筛选掉
        if (area < 1000)
            continue;

        //轮廓近似，作用较小，approxPolyDP函数有待研究
        double epsilon = 0.001*arcLength(contours[i], true);
        Mat approx;
        approxPolyDP(contours[i], approx, epsilon, true);

        //找到最小矩形，该矩形可能有方向
        RotatedRect rect = minAreaRect(contours[i]);

        //计算高和宽
        int m_width = rect.boundingRect().width;
        int m_height = rect.boundingRect().height;

        //筛选那些太细的矩形，留下扁的
        if (m_height > m_width * 1.2)
            continue;

        //符合条件的rect添加到rects集合中
        rects.push_back(rect);

    }
    return rects;
}

void detect(Mat img)
{
    //1.转化成灰度图
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    //2.形态学变换的预处理，得到可以查找矩形的轮廓
    Mat dilation = preprocess(gray);

    //3.查找和筛选文字区域
    vector<RotatedRect> rects = findTextRegion(dilation);

    //4.用绿线画出这些找到的轮廓
    for each (RotatedRect rect in rects)
    {
        Point2f P[4];
        rect.points(P);
        for (int j = 0; j <= 3; j++)
        {
            line(img, P[j], P[(j + 1) % 4], Scalar(0,255,0), 2);
        }
    }

    //5.显示带轮廓的图像
    imshow("img", img);
    imwrite("imgDrawRect.jpg", img);

    waitKey(0);
}
