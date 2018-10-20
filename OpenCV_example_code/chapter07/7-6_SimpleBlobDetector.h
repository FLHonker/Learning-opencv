// C/C++ header file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 7-6_SimpleBlobDetector.h
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: blob检测类
# @Create Time: 2018-10-19 11:58:43
# @Last Modified: 2018-10-19 11:58:43
***********************************************/
class SimpleBlobDetector : public FeatureDetector
{
public:
    struct Params 
    {
        Params();
        // 二值化间隔
        flost thresholdStep;
        // 二值化起始区间
        float minThreshold;
        // 二值化终止空间
        float maxThreshold;
        size_t minRepeatability;
        float minDistBetweenBlobs;
        // 颜色特征选择
        bool filterByColor;
        // 亮度区域标志
        uchar blobColor;
        float minArea, maxArea;
        bool filterByCircularity;
        // 目标半径
        float minCircularity, maxCircularity;
        bool filterByInteria;
        // 凸包限定
        float minIntertiaRatio, maxInteriaRatio;
        bool filterByConvexity;
        float minConvexity, maxConvexity;
    };
    SimpleBlobDetector(const SimpleBlobDetector::Params &parameters = SimpleBlobDetector::Params());
protected:
    // ...

}
