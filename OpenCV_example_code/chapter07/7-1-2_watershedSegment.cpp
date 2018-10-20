// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-1-2_watershedSegment.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 分水岭的分割与合并
# @Create Time: 2018-10-17 14:30:05
# @Last Modified: 2018-10-17 14:30:05
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

// 分水岭分割显示
Mat displaySegResult(Mat &segments, int numOfSegments, Mat &image)
{
    Mat wshed(segments.size(), CV_8UC3);
    // 创建颜色分量
    vector<Vec3b> colorTab;
    for(int i = 0; i < numOfSegments; ++i)
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);
        colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // 应用不同颜色对每个部分
    for(int i = 0; i < segments.rows; ++i)
    {
        for(int j = 0; j < segments.cols; ++j)
        {
            int index = segments.at<int>(i, j);
            if(index == 1)
                wshed.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
            else if(index <= 0 || index > numOfSegments)
                wshed.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
            else
                wshed.at<Vec3b>(i, j) = colorTab[index-1];
        }
    }
    if(image.dims > 0)
        wshed = wshed * 0.5 + image * 0.5;
    
    return wshed;
}

// 7-1 分水岭的分割
Mat watershedSegment(Mat &image, int &numOfSegments)
{
    Mat gray, ret;
    cvtColor(image, gray, CV_BGR2GRAY);
    imshow("grayImage", gray);
    // 阈值化
    threshold(gray, ret, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
    imshow("OTSU", ret);
    // 形态学开操作
    morphologyEx(ret, ret, MORPH_OPEN, Mat::ones(9,9,CV_8SC1), Point(4,4), 2);
    imshow("Morphological open", ret);
    // 距离变换
    Mat distTransformed(ret.size(), CV_32FC1);
    distanceTransform(ret, distTransformed, CV_DIST_L2, 3);
    // 归一化
    normalize(distTransformed, distTransformed, 0.0, 1, NORM_MINMAX);
    imshow("distance transformed", distTransformed);
    // 阈值化分割图像
    threshold(distTransformed, distTransformed, 0.1, 1, CV_THRESH_BINARY);
    // 归一化统计图像到0~255
    normalize(distTransformed, distTransformed, 0.0, 255.0, NORM_MINMAX);
    distTransformed.convertTo(distTransformed, CV_8UC1);
    imshow("thresholded distance transformation", distTransformed);
    // 计算标记的分割块
    int i, j, compCount = 0;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(distTransformed, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    if(contours.empty())
        return Mat();
    Mat markers(distTransformed.size(), CV_32S);
    markers = Scalar::all(0);
    int idx = 0;
    // 绘制区域块
    for(; idx >= 0; idx = hierarchy[idx][0], compCount++)
    {
        drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);
    }
    if(compCount == 0)
        return Mat();
    // 计算算法的时间复杂度
    double t = (double)getTickCount();
    watershed(image, markers);
    t = (double)getTickCount() - t;
    cout << "watershed time: " << t * 1000 / getTickFrequency() << "ms" <<endl;
    Mat wshed = displaySegResult(markers, compCount, image);
    imshow("watershed transform", wshed);
    numOfSegments = compCount;
    return markers;
}

// 7-2 分割合并
void segMerge(Mat &image, Mat &segments, int &numSeg)
{
    // 对每一个分割部分进行像素统计
    vector<Mat> samples;
    // 统计数据更新
    int newNumSeg = numSeg;
    // 初始化分割部分
    for(int i = 0; i <= numSeg; ++i)
    {
        Mat sampleImg;
        samples.push_back(sampleImg);
    }

    // 统计每一个部分
    for(int i = 0; i < segments.rows; ++i)
    {
        for(int j = 0; j < segments.cols; ++j)
        {
            // 检查每个像素的归属
            int index = segments.at<uchar>(i, j);
            if(index >= 0 && index < numSeg)
                samples[index].push_back(image(Rect(j,i,1,1)));
        }
    }
    // 创建直方图
    vector<MatND> hist_bases;
    Mat hsv_base;
    // 直方图参数设置
    int h_bins = 35;
    int s_bins = 30;
    int histSize[] = { h_bins, s_bins };
    // hue 变换范围 0 to 256, saturation 变换范围0 to 180
    float h_ranges[] = { 0, 256 };
    float s_ranges[] = { 0, 180 };
    const float* ranges[] = { h_ranges, s_ranges };
    // 使用第0与1通道
    int channels[] = { 0, 1 };
    // 直方图生成
    MatND hist_base;

    for (int c = 1; c < numSeg; c++) {
        if (samples[c].dims > 0) {
            // 将区域部分转换成hsv
            cvtColor(samples[c], hsv_base, CV_BGR2HSV);
            // 直方图统计
            calcHist(&hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
            // 直方图归一化
            normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());
            // 添加到统计集
            hist_bases.push_back(hist_base);
        } else {
            hist_bases.push_back(MatND());
        }

        hist_base.release();
    }

    double similarity = 0;
    vector<bool> mearged;

    for (int k = 0; k < hist_bases.size(); k++) {
        mearged.push_back(false);
    }

    // 统计每一个部分的直方图相似
    for (int c = 0; c < hist_bases.size(); c++) {
        for (int q = c + 1; q < hist_bases.size(); q++) {
            if (!mearged[q]) {
                if (hist_bases[c].dims > 0 && hist_bases[q].dims > 0) {
                    similarity = compareHist(hist_bases[c], hist_bases[q], CV_COMP_BHATTACHARYYA);

                    if (similarity > 0.8) {
                        mearged[q] = true;

                        if (q != c) {
                            //区域部分减少
                            newNumSeg--;

                            for (int i = 0; i < segments.rows; i++) {
                                for (int j = 0; j < segments.cols; j++) {
                                    int index = segments.at<int>(i, j);

                                    // 合并
                                    if (index == q) {
                                        segments.at<int>(i, j) = c;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    numSeg = newNumSeg;
}

int main()
{
    cv::Mat srcImage = imread("../../imgdata/sea.jpg");

    if (!srcImage.data)
        return -1;

    cv::imshow("Original Image", srcImage);
    // 转换成hsv图像
    cv::Mat hsv;
    cv::cvtColor(srcImage, hsv, CV_BGR2HSV);
    cv::imshow("hsv", hsv);
    // 分水岭操作
    int numOfSegments = 0;
    cv::Mat segments = watershedSegment(srcImage, numOfSegments);
    // 分割排序减少过分割
    segMerge(srcImage, segments, numOfSegments);
    // 显示分割图像
    cv::Mat wshed = displaySegResult(segments, numOfSegments, srcImage);
    // 显示分割后合并图像
    cv::Mat wshedWithImage = displaySegResult(segments, numOfSegments, srcImage);
    cv::imshow("Merged segments", wshed);
    cv::imshow("Merged segments with image", wshedWithImage);
    cv::waitKey(0);
    return 0;
}
