// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-17-21_rotatedTextCorrection.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 旋转文本图像校正
# @Create Time: 2018-10-06 17:16:36
# @Last Modified: 2018-10-06 17:16:36
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

/**
 * @brief 旋转文本校正
 * 
 * @param src
 * @param dst
 */
void rotatedTextCorrection(const Mat src, Mat& dst)
{
    /***************** 1.图像DFT尺寸变换 *******************/
    
    const int nRows = src.rows, nCols = src.cols;
    cout << "srcImage row: " << nRows <<endl;
    cout << "srcImage col: " << nCols <<endl;
    // 获取DFT尺寸
    int cRows = getOptimalDFTSize(nRows);
    int cCols = getOptimalDFTSize(nCols);
    cout << "DFT srcImage row: " << cRows <<endl;
    cout << "DFT srcImage col: " << cCols <<endl;
    // 复制图像，超过边界区域填充为0
    Mat sizeConvMat;
    copyMakeBorder(src, sizeConvMat, 0, cRows - nRows, 
        0, cCols - nCols, BORDER_CONSTANT, Scalar::all(0));
    imshow("sizeConvMat", sizeConvMat);

    /******************* 2.图像DFT变换 ********************/
    
    // 通道组建立
    Mat groupMats[] = { Mat_<float>(sizeConvMat), Mat::zeros(sizeConvMat.size(), CV_32F) };
    Mat mergeMat;
    // 通道合并
    merge(groupMats, 2, mergeMat);
    // DFT变换
    dft(mergeMat, mergeMat);
    // 分离通道
    split(mergeMat, groupMats);
    // 计算幅值
    magnitude(groupMats[0], groupMats[1], groupMats[0]);
    Mat magnitudeMat = groupMats[0].clone();
    // 归一化操作，幅值加1
    magnitudeMat += Scalar::all(1);
    // 对数变换
    log(magnitudeMat, magnitudeMat);
    // 归一化
    normalize(magnitudeMat, magnitudeMat, 0, 1, CV_MINMAX);
    // 图像类型转换
    magnitudeMat.convertTo(magnitudeMat, CV_8UC1, 255, 0);
    imshow("magnitudeMat", magnitudeMat);

    /****************** 3.频域中心移动 ********************/
    /**
     * 方法：将图像等分为4份，然后将区域进行互调，完成中心移动。
     */

    // 中心平移
    int cx = magnitudeMat.cols / 2;
    int cy = magnitudeMat.rows / 2;
    Mat tmp;
    // Top-Left —— 为每一个象限创建ROI
    Mat q0(magnitudeMat, Rect(0, 0, cx, cy));
    // Top-Right
    Mat q1(magnitudeMat, Rect(cx, 0, cx, cy));
    // Bottom-Left
    Mat q2(magnitudeMat, Rect(0, cy, cx, cy));
    // Bottom-Right
    Mat q3(magnitudeMat, Rect(cx, cy, cx, cy));
    // 交换象限(Top-Left <--> Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    // 交换象限(Top-Right <--> Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    imshow("magnitudeMat2", magnitudeMat);

    /*************** 4.倾斜角检测 ******************/
    
    // 固定阈值二值化处理
    Mat binaryMagnMat;
    threshold(magnitudeMat, binaryMagnMat, 132, 255, CV_THRESH_BINARY);
    imshow("binaryMagnMat", binaryMagnMat);
    /// 霍夫变换
    vector<Vec2f> lines;
    binaryMagnMat.convertTo(binaryMagnMat, CV_8UC1, 255, 0);
    HoughLines(binaryMagnMat, lines, 1, CV_PI/180, 100, 0, 0);
    // 检测线个数
    cout << "lines.size:" << lines.size() <<endl;
    Mat houghMat(binaryMagnMat.size(), CV_8UC3);
    // 绘制检测线
    for(size_t i = 0; i < lines.size(); ++i)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        // 坐标变换生成线表达式
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * a);
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * a);
        line(houghMat, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
    }
    imshow("houghMat", houghMat);
    float theta = 0;
    // 检测线角度判断
    for(size_t i = 0; i < lines.size(); ++i)
    {
        float thetaTemp = lines[i][1] * 180 / CV_PI;
        if(thetaTemp > 0 && thetaTemp < 90)
        {
            theta = thetaTemp;
            break;
        }
    }
    // 角度转换
    float angelT = nRows * tan(theta / 180 * CV_PI) / nCols;
    theta = atan(angelT) * 180 / CV_PI;
    cout << "theta: " << theta <<endl;

    /***************** 5.仿射变换矫正*******************/

    // 取图像中心
    Point2f centerPoint = Point2f(nCols / 2, nRows / 2);
    double scale = 1;
    // 计算旋转矩阵
    Mat warpMat = getRotationMatrix2D(centerPoint, theta, scale);
    // 仿射变换
    dst.create(nRows, nCols, src.type());
    warpAffine(src, dst, warpMat, dst.size());
    imshow("dstImage", dst);
}

// test
int main()
{
    Mat srcImg = imread("../../imgdata/rotatedText.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(srcImg.empty())
        return -1;
    Mat dstImg;
    rotatedTextCorrection(srcImg, dstImg);
    waitKey(0);
    return 0;
}
