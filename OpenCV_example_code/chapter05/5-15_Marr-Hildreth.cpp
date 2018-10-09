// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 5-15_Marr-Hildreth.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 改进边缘检测算子——Marr-Hildreth
# @Create Time: 2018-10-09 15:26:48
# @Last Modified: 2018-10-09 15:26:48
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void marrEdge(const Mat src, Mat& dst, int kerValue, double delta)
{
    // 半径
    int kerLen = kerValue / 2;
    // 计算LoG算子
    Mat kernel = Mat_<double>(kerValue, kerValue);
    // 滑窗
    for(int i = -kerLen; i <= kerLen; ++i) 
    {
        for(int j = -kerLen; j <= kerLen; ++j)
        {
            // 生成核因子
            kernel.at<double>(i + kerLen, j + kerLen) = 
                exp(-((pow(i, 2) + pow(j, 2)) / (pow(delta, 2)*2))) * 
                (((pow(i, 2) + pow(j, 2) - 2 * pow(delta, 2)) / (2 * pow(delta, 4))));
        }
    }
    // 设置参数
    int kerOffset = kerValue / 2;
    Mat laplacian = Mat_<double>(src.rows - kerOffset*2, src.cols - kerOffset*2);
    dst = Mat::zeros(src.rows - kerOffset*2, src.cols - kerOffset*2, src.type());
    double sumLaplacian;
    // 遍历计算卷积图像的拉普拉斯算子
    for(int i = kerOffset; i < src.rows - kerOffset; ++i)
    {
        for(int j = kerOffset; j < src.cols- kerOffset; ++j)
        {
            sumLaplacian = 0;
            for(int k = -kerLen; k <= kerLen; ++k)
            {
                for(int m = -kerLen; m <= kerLen; ++m)
                {
                    // 计算图像卷积
                    sumLaplacian += src.at<uchar>(i + k, j + m) * 
                        kernel.at<double>(kerLen + k, kerLen + m);
                }
            }
            // 生成Laplace结果
            laplacian.at<double>(i - kerOffset, j - kerOffset) = sumLaplacian;
        }
    }
    
    // 过零点交叉，寻找边缘像素
    for(int y = 1; y < dst.rows - 1; ++y)
    {
        for(int x = 1; x < dst.cols - 1; ++x)
        {
            dst.at<uchar>(y, x) = 0;
            // 邻域判定
            if(laplacian.at<double>(y-1, x) * laplacian.at<double>(y+1, x) < 0)
                dst.at<uchar>(y, x) = 255;
            if(laplacian.at<double>(y, x-1) * laplacian.at<double>(y, x+1) < 0)
                dst.at<uchar>(y, x) = 255;
            if(laplacian.at<double>(y+1, x-1) * laplacian.at<double>(y-1, x+1) < 0)
                dst.at<uchar>(y, x) = 255;
            if(laplacian.at<double>(y-1, x-1) * laplacian.at<double>(y+1, x+1) < 0)
                dst.at<uchar>(y, x) = 255;
        }
    }
}

int main()
{
    Mat srcImg = imread("../../imgdata/building.jpg", 0);
    if(!srcImg.data)
        return -1;
    Mat edgeImg;
    marrEdge(srcImg, edgeImg, 9, 1.6);
    imshow("srcImage", srcImg);
    imshow("edgsImage", edgeImg);
    waitKey(0);
    return 0;
}
