// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 2-30_multiImagesWin.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 单窗口显示多幅图像
# @Create Time: 2018-09-10 17:09:38
# @Last Modified: 2018-09-10 17:09:38
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

void showMultiImages(const vector<Mat> &srcImg, Size imgSize)
{
    int nNumImages = srcImg.size();
    Size nSizeWindows;
    if(nNumImages > 12)
    {
        cout << "Not more than 12 images!" <<endl;
        return;
    }
    // 根据图片序列数量来确定分割小窗口的形态
    switch(nNumImages)
    {
    case 1: nSizeWindows = Size(1, 1); break;
    case 2: nSizeWindows = Size(2, 1); break;
    case 3:
    case 4: nSizeWindows = Size(2, 2); break;
    case 5:
    case 6: nSizeWindows = Size(3, 2); break;
    case 7:
    case 8: nSizeWindows = Size(4, 2); break;
    case 9: nSizeWindows = Size(3, 3); break;
    default: nSizeWindows = Size(4, 3); break;
    }
    // 设置小图像尺寸、间隙、边界
    int nShowImageSize = 200;
    int nSplitLineSize = 15;
    int nAroundLineSize = 50;
    // 创建输出图像，图形大小根据输入源来确定
    const int imagesWidth = nShowImageSize * nSizeWindows.width + 
        nAroundLineSize + (nSizeWindows.width - 1) * nSplitLineSize;
    const int imagesHeight = nShowImageSize * nSizeWindows.height + 
        nAroundLineSize + (nSizeWindows.height - 1) * nSplitLineSize;
    cout << "images size: " << imagesWidth << " x " << imagesHeight <<endl;
    Mat showWindowImages(imagesWidth, imagesHeight, CV_8UC3, Scalar(0,0,0));
    // 提取对应小图像的左上角坐标x,y
//    in posX = (showWindowImages.cols - (nShowImageSize * nSizeWindows.width + (nSizeWindows.width - 1) * nSplitLineSize)) / 2;
    int posX = nAroundLineSize / 2;
//    int posY = (showWindowImages.rows - (nShowImageSize * nSizeWindows.height + (nSizeWindows.height - 1) * nSplitLineSize)) / 2;
    int posY = nAroundLineSize / 2;
    cout << "posX = " << posX << ", posY = " << posY <<endl;
    int tempPosX = posX, tempPosY = posY;
    // 将每一小幅图像整合成大图像
    for(int i = 0; i < nNumImages; ++i)
    {
        // 小图像坐标变换
        if(i % nSizeWindows.width == 0 && tempPosX != posX)
        {
            tempPosX = posX;
            tempPosY += (nSplitLineSize + nShowImageSize);
        }
        // 利用Rect区域将小图像置于大图中相应区域
        Mat tempImg = showWindowImages(Rect(tempPosX, tempPosY, nShowImageSize, nShowImageSize));
        // 利用resize()实现图像缩放
        resize(srcImg[i], tempImg, Size(nShowImageSize, nShowImageSize));
        tempPosX += (nSplitLineSize + nShowImageSize);
    }
    imshow("showWindowImages", showWindowImages);
}

int main()
{
    // 图像源输入
    std::vector<Mat> srcImgs(9);
    srcImgs[0] = imread("../../imgdata/aero1.jpg");
    srcImgs[1] = imread("../../imgdata/aero1.jpg");
    srcImgs[2] = imread("../../imgdata/aero1.jpg");
    srcImgs[3] = imread("../../imgdata/aero3.jpg");
    srcImgs[4] = imread("../../imgdata/aero3.jpg");
    srcImgs[5] = imread("../../imgdata/apple.jpg");
    srcImgs[6] = imread("../../imgdata/apple.jpg");
    srcImgs[7] = imread("../../imgdata/apple.jpg");
    srcImgs[8] = imread("../../imgdata/apple.jpg");
    // 判断当前vector读入的正确性
    for(int i = 0; i < srcImgs.size(); ++i)
    {
        if(!srcImgs[i].data)
        {
            cout << "read error!" <<endl;
            return -1;
        }
    }
    // 调节单窗口显示图像
    showMultiImages(srcImgs, Size(520, 400));
    waitKey(0);
    return 0;
}
