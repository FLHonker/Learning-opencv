// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-7-9_HOG.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: ---
# @Create Time: 2018-10-26 11:25:34
# @Last Modified: 2018-10-26 11:25:34
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <vector>

using namespace cv;
using namespace std;

#define NBINS 9
#define THETA 180/NBINS
#define CELLSIZE 20
#define BLOCKSIZE 2
#define R (CELLSIZE * BLOCKSIZE * 0.5)

// 8-7 计算积分图
vector<Mat> CalculateIntegralHOG(Mat& src)
{
    // Sobel边缘检测
    Mat sobelMatX, sobelMatY;
    Sobel(src, sobelMatX, CV_32F, 1, 0);
    Sobel(src, sobelMatY, CV_32F, 0, 1);
    vector<Mat> bins(NBINS);
    for(int i = 0; i < NBINS; ++i)
    {
        bins[i] = Mat::zeros(src.size(), CV_32F);
    }
    Mat magnMat, angleMat;
    // 坐标转换
    cartToPolar(sobelMatX, sobelMatY, magnMat, angleMat, true);
    // 角度变换
    add(angleMat, Scalar(180), angleMat, angleMat < 0);
    add(angleMat, Scalar(-180), angleMat, angleMat >= 180);
    angleMat /= THETA;
    for(int y = 0; y < src.rows; ++y)
    {
        for(int x = 0; x < src.cols; ++x)
        {
            // 计算bins下幅值
            int ind = angleMat.at<float>(y,x);
            bins[ind].at<float>(y,x) += magnMat.at<float>(y,x);
        }
    }
    // 生成积分图像
    vector<Mat> integrals(NBINS);
    for(int i = 0; i < NBINS; ++i)
    {
        integral(bins[i], integrals[i]);
    }
    return integrals;
}

// 8-8 计算单个cell HOG特征
void cacHOGinCell(Mat& HOGCellMat, Rect roi, vector<Mat> &integrals)
{
    // 实现快速积分HOG
    int x0 = roi.x, y0 = roi.y;
    int x1 = x0 + roi.width, y1 = y0 + roi.height;
    for(int i = 0; i < NBINS; ++i)
    {
        // 根据矩阵的上下左右坐标计算
        Mat integral = integrals[i];
        float a = integral.at<double>(y0, x0);
        float b = integral.at<double>(y1, x1);
        float c = integral.at<double>(y0, x1);
        float d = integral.at<double>(y1, x0);
        HOGCellMat.at<float>(0, i) = (a + b) - (c + d);
    }
}

// 8-9 获取HOG直方图
Mat getHOG(Point pt, vector<Mat> &integrals)
{
    // 判断当前点的位置是否符合条件
    if(pt.x - R < 0 || pt.y - R < 0 || 
       pt.x + R >= integrals[0].cols || pt.y + R >= integrals[0].rows)
    {
        return Mat();
    }
    // 直方图
    Mat hist(Size(NBINS*BLOCKSIZE*BLOCKSIZE, 1), CV_32F);
    Point t1(0, pt.y-R);
    int c = 0;
    // 遍历块
    for(int  i = 0; i < BLOCKSIZE; ++i)
    {
        t1.x = pt.x - R;
        for(int j = 0; j < BLOCKSIZE; ++j)
        {
            // 获取当前窗口，计算局部直方图
            Rect roi(t1, t1+Point(CELLSIZE, CELLSIZE));
            // 计算当前bins下直方图
            Mat hist_temp = hist.colRange(c, c+NBINS);
            cacHOGinCell(hist_temp, roi, integrals);
            // cell步长尺寸
            t1.x += CELLSIZE;
            c += NBINS;
        }
        t1.y = CELLSIZE;
    }
    // 归一化L2范数
    normalize(hist, hist, 1, 0, NORM_L2);
    return hist;
}

// 8-9 计算HOG特征
vector<Mat> cacHOGFeature(Mat srcImage)
{
    Mat grayImage;
    vector<Mat> HOGMatVector;
    cvtColor(srcImage, grayImage, CV_RGB2GRAY);
    grayImage.convertTo(grayImage, CV_8UC1);
    // 积分图像生成
    vector<Mat> integrals = CalculateIntegralHOG(grayImage);
    Mat image = grayImage.clone();
    image *= 0.5;
    // Block遍历
    Mat HOGBlockMat(Size(NBINS, 1), CV_32F);

    for (int y = CELLSIZE / 2; y < grayImage.rows; y += CELLSIZE) 
    {
        for (int x = CELLSIZE / 2; x < grayImage.cols; x += CELLSIZE) 
        {
            // 获取当前窗口HOG
            Mat hist = getHOG(Point(x, y), integrals);

            if (hist.empty())
                continue;

            HOGBlockMat = Scalar(0);

            for (int i = 0; i < NBINS; i++) 
            {
                for (int j = 0; j < BLOCKSIZE; j++) 
                {
                    HOGBlockMat.at<float>(0, i) += hist.at<float>(0, i + j * NBINS);
                }
            }

            // L2范数归一化
            normalize(HOGBlockMat, HOGBlockMat, 1, 0, CV_L2);
            HOGMatVector.push_back(HOGBlockMat);
            Point center(x, y);

            // 绘制HOG特征图
            for (int i = 0; i < NBINS; i++) 
            {
                double theta = (i * THETA + 90.0) * CV_PI / 180.0;
                Point rd(CELLSIZE * 0.5 * cos(theta), CELLSIZE * 0.5 * sin(theta));
                Point rp = center - rd;
                Point lp = center - -rd;
                line(image, rp, lp, Scalar(255 * HOGBlockMat.at<float>(0, i), 255, 255));
            }
        }
    }

    imshow("out", image);
    return HOGMatVector;
}

int main()
{
    Mat srcImage = imread("../../imgdata/hand1.jpg");
    if (srcImage.empty())
        return -1;
    imshow("srcImage ", srcImage);
    vector<Mat> HOGFeatureMat = cacHOGFeature(srcImage);
    waitKey(0);
    return 0;
}
