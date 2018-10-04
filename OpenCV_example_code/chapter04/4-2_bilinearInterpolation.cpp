// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-2_bilinearInterpolation.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 双线性插值
# @Create Time: 2018-10-02 19:41:39
# @Last Modified: 2018-10-02 19:41:39
***********************************************/
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp> 
#include <iostream> 
// 实现双线性插值图像缩放
cv::Mat BilinearInterpolation(cv::Mat srcImage)
{
	CV_Assert(srcImage.data != NULL);
	int srcRows = srcImage.rows;
	int srcCols = srcImage.cols;
	int srcStep = srcImage.step;
	// 构建目标图像
	cv::Mat dstImage = cv::Mat(
		cv::Size(150, 150), srcImage.type(),
		cv::Scalar::all(0));
	int dstRows = dstImage.rows;
	int dstCols = dstImage.cols;
	int dstStep = dstImage.step;
	// 数据定义及转换
	IplImage src = srcImage;
	IplImage dst = dstImage;
	std::cout << "srcCols:" << srcCols << " srcRows:" <<
		srcRows << "srcStep:" << srcStep << std::endl;
	std::cout << "dstCols:" << dstCols << " dstRows:" <<
		dstRows << "dstStep:" << dstStep << std::endl;
	// 坐标定义
	float srcX = 0, srcY = 0;
	float t1X = 0, t1Y = 0, t1Z = 0;
	float t2X = 0, t2Y = 0, t2Z = 0;
	for (int j = 0; j < dstRows - 1; j++)
	{
		for (int i = 0; i < dstCols - 1; i++)
		{
			// 缩放映射关系
			srcX = (i + 0.5)*((float)srcCols) / (dstCols)-0.5;
			srcY = (j + 0.5)*((float)srcRows) / (dstRows)-0.5;
			int iSrcX = (int)srcX;
			int iSrcY = (int)srcY;
			// 三通道求邻域加权值1
			t1X = ((uchar*)(src.imageData + srcStep*iSrcY))[
				iSrcX * 3 + 0] * (1 - std::abs(srcX - iSrcX)) +
				((uchar*)(src.imageData + srcStep*iSrcY))[
                (iSrcX + 1) * 3 + 0] * (srcX - iSrcX);
            t1Y = ((uchar*)(src.imageData + srcStep*iSrcY))[
				iSrcX * 3 + 1] * (1 - std::abs(srcX - iSrcX)) +
				((uchar*)(src.imageData + srcStep*iSrcY))[
				(iSrcX + 1) * 3 + 1] * (srcX - iSrcX);

			t1Z = ((uchar*)(src.imageData + srcStep*iSrcY))[
				iSrcX * 3 + 2] * (1 - std::abs(srcX - iSrcX)) +
				((uchar*)(src.imageData + srcStep*iSrcY))[
				(iSrcX + 1) * 3 + 2] * (srcX - iSrcX);
			// 三通道求邻域加权值2
			t2X = ((uchar*)(src.imageData + srcStep*(
				iSrcY + 1)))[iSrcX * 3] * (1 - std::abs(srcX - iSrcX))
				+ ((uchar*)(src.imageData + srcStep*(
				iSrcY + 1)))[(iSrcX + 1) * 3] * (srcX - iSrcX);
			t2Y = ((uchar*)(src.imageData + srcStep*(
				iSrcY + 1)))[iSrcX * 3 + 1] * (1 - std::abs(srcX - iSrcX))
				+ ((uchar*)(src.imageData + srcStep*(
				iSrcY + 1)))[(iSrcX + 1) * 3 + 1] * (srcX - iSrcX);
			t2Z = ((uchar*)(src.imageData + srcStep*(
				iSrcY + 1)))[iSrcX * 3 + 2] * (1 - std::abs(srcX - iSrcX))
				+ ((uchar*)(src.imageData + srcStep*(iSrcY + 1)))[(
				iSrcX + 1) * 3 + 2] * (srcX - iSrcX);
			// 根据公式求解目标图像加权
			((uchar*)(dst.imageData + dstStep*j))[i * 3] =	t1X*(1 - std::abs(srcY - iSrcY)) + t2X*(
			                                                std::abs(srcY - iSrcY));
			((uchar*)(dst.imageData + dstStep*j))[i * 3 + 1] = t1Y*(1 - std::abs(srcY - iSrcY)) + t2Y*(
			                                                std::abs(srcY - iSrcY));
			((uchar*)(dst.imageData + dstStep*j))[i * 3 + 2] = t1Z*(1 - std::abs(srcY - iSrcY)) + t2Z*(
			                                                std::abs(srcY - iSrcY));
		}
		// 列操作
		((uchar*)(dst.imageData + dstStep*j))[(dstCols - 1) * 3] =
			((uchar*)(dst.imageData + dstStep*j))[(dstCols - 2) * 3];
		((uchar*)(dst.imageData + dstStep*j))[(dstCols - 1) * 3 +
			1] = ((uchar*)(dst.imageData + dstStep*j))[(
				dstCols - 2) * 3 + 1];
		((uchar*)(dst.imageData + dstStep*j))[(dstCols - 1) * 3
			+ 2] = ((uchar*)(dst.imageData + dstStep*j))[(
				dstCols - 2) * 3 + 2];
	}
	// 行操作
	for (int i = 0; i < dstCols * 3; i++)
	{
		((uchar*)(dst.imageData + dstStep*(dstRows - 1)))[i] =
			((uchar*)(dst.imageData + dstStep*(dstRows - 2)))[i];
	}
	return  dstImage;
}

int main()
{
	cv::Mat srcImage = cv::imread("..\\images\\flower3.jpg");
	if (!srcImage.data)
		return -1;
	cv::Mat dstImage = BilinearInterpolation(srcImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("dstImage", dstImage);
	cv::waitKey(0);
	return 0;
}
