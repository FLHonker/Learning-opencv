// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-6_MOG2_set.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: MOG2成员函数参数设定
# @Create Time: 2018-11-20 10:55:41
# @Last Modified: 2018-11-20 10:55:41
***********************************************/

// 阴影去除，默认打开
pMOG2->setDetectShadows(true);
// 背景建模影响帧数，默认为500
pMOG2->setHistory(1000);
// 模型匹配阈值
pMOG2->setVarThreshold(50);
// 阴影阈值
pMOGg2->setShadowThreshold(0.7);
// 前景中的模型参数，设置为0示背景，255表示前景，默认值为127;
pMog2->setShadowValue(127);
// 背景阈值设定backgroundRatio*history
pMOG2->setBackgroundRatio(2);
// 设置阈值降低的复杂性
pMOG2->setComplexityReductionThreshold(0.02);
// 高斯混合模型组件数量
pMOG2->setNMixtures(100);
// 设置每个高斯组件的初始方差
pMOG2->setVarInit(0.5);
// 新模型匹配阈值
pMOG2->setVarThresholdGen(9);

