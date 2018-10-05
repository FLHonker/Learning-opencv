// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 4-10_boxFilter.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: boxFilter滤波器实现
# @Create Time: 2018-10-04 21:30:11
# @Last Modified: 2018-10-04 21:30:11
***********************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 盒子滤波器基类生成函数
cv::Ptr<cv::FilterEngine> cv::createBoxFilter(int srcType, int dstType, 
    Size ksize, Point anchor, bool normalize, int borderType)
{
    // 基类参数设置，图像深度
    int sdepth = CV_MAT_DEPTH(srcType);
    int cn = CV_MAT_CN(srcType), sumType = CV_64F;
    if(sdepth <= CV_32S && (!normalize || ksize.width * ksize.height <= 
      (sdepth == CV_8U ? (1<<23) : sdepth == CV_16U ? (1<<15) : (1<<16))))
        sumType = CV_32S;
    sumType = CV_MAKETYPE(sumType, cn);
    // 获取滤波器类中行滤波器信息
    Ptr<BaseRowFilter> rowFilter = getRowSumFilter(srcType, sumType, ksize.width, anchor.x);
    // 获取滤波器类中列滤波器信息
    Ptr<BaseColumnFilter> columnFilter = getColumnFilter(sumType, dstType, ksize.height, 
                                        anchor.y, normalize ? 1.0 / (ksize.width * ksize.height) : 1);
    // 返回滤波器基类生成
    return Ptr<FilterEngine>(new FilterEngine(
        Ptr<BaseFilter>(0), rowFilter, columnFilter,
        srcType, dstType, sumType, borderType));                                         ))
}

// 盒子滤波器实现
void cv::boxFilter(InputArray _src_, OutputArray _dst, 
                   int ddepth, Size ksize, Point anchor, 
                   bool normalize, int borderType)
{
    // 获取矩阵相关信息
    Mat src = _src.getMat();
    int sdepth = src.depth(), cn = src.channels();
    if(ddepth < 0)
        ddepth = sdepth;
    _dst.create(src.size(), CV_MAKETYPE(ddepth, cn));
    Mat dst = _dst.getMat();
    // 设定边界因子
    if(borderType != BORDER_CONSTANT && normalize)
    {
        if(src.rows == 1)
            ksize.height = 1;
        if(src.cols == 1)
            ksize.width  = 1;
    }
    // 生成盒子滤波器
    Ptr<FilterEngine> f = createBoxFilter(src.type(), 
        dst.type(), ksize, anchor, normalize, borderType);
    // 用apply方法实现相关滤波操作
    f->apply(src, dst);
}
