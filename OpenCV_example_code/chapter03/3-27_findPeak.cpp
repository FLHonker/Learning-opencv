// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 3-27_findPeak.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 投影峰谷查找
# 步週：
# (1) 计算差分向量: D = S(i+1) - S(i) ；(2) 判定向量符号: D_s = sign(D)；
# (3) 遍历差分向量； (4) 差分遍历判定峰谷。
# 投影曲线的波峰/波谷是通过判定其一阶导数为0点，二阶导数为正或负值来确定的。
# @Create Time: 2018-09-17 20:27:29
# @Last Modified: 2018-09-17 20:27:29
***********************************************/
#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

// 计算图像的波峰
void findPeak(Mat srcImg, vector<int>& resultVec)
{
    Mat verMat;
    Mat resMat = srcImg.clone();
    // 阈值化操作
    int thresh = 30, threshType = 0;
    // 预设最大值
    const int maxVal = 255;
    // 固定阈值化操作
    threshold(srcImg, srcImg, thresh, maxVal, threshType);
    srcImg.convertTo(srcImg, CV_32FC1);
    // 计算垂直投影
    reduce(srcImg, verMat, 0, CV_REDUCE_SUM);
    //std::cout << verMat << std::endl;
    // 遍历求差分符号函数
    float* iptr = (float*)verMat.data + 1;
    vector<int> tempVec(verMat.cols - 1, 0);
    for(int i = 1; i < verMat.cols - 1; ++i, ++iptr)
    {
        if(*(iptr+1) - *iptr > 0)
            tempVec[i] = 1;
        else if(*(iptr+1) - *iptr < 0)
            tempVec[i] = -1;
        else
            tempVec[i] = 0;
    }
    // 对符号函数进行遍历
    for(int i = tempVec.size()-1; i >= 0; --i)
    {
        if(tempVec[i] == 0 && i == tempVec.size()-1)
            tempVec[i] = 1;
        else if(tempVec[i] == 0)
        {
            if(tempVec[i+1] >= 0)
                tempVec[i] = 1;
            else
                tempVec[i] = -1;
        }
    }
    // 波峰判断输出
    for(vector<int>::size_type i = 0; i != tempVec.size()-1; ++i)
        if(tempVec[i+1] - tempVec[i] == -2)
            resultVec.push_back(i + 1);
    // 输出波峰位置
    for(int i = 0; i < resultVec.size(); ++i)
    {
        std::cout << resultVec[i] << " ";
        // 波峰位置为255
        for(int j = 0; j < resMat.rows; ++j)
            resMat.at<uchar>(j, resultVec[i]) = 255;
    }
    imshow("resMat", resMat);
}

int main()
{
    Mat srcImg = imread("../../imgdata/mountainwater.jpg");
    if(!srcImg.data)
        return -1;
    imshow("srcImage", srcImg);
    vector<int> resultVec;
    findPeak(srcImg, resultVec);
    waitKey(0);
    return 0;
}
