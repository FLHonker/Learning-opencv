// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-6_HOGDescriptor.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: HOGDescriptor描述类
# @Create Time: 2018-10-26 11:24:38
# @Last Modified: 2018-10-26 11:24:38
***********************************************/
struct CV_EXPORTS HOGDescriptor {
    // 高斯平滑参数
    enum { DEFAULT_WIN_SIGMA = -1 };
    // 检测窗口的最大数量
    enum { DEFAULT_NLEVELS = 64 };
    // 描述符存储格式
    enum { DESCR_FORMAT_ROW_BY_ROW, DESCR_FORMAT_COL_BY_COL };
    // 创建了特征描述符和检测器
    HOGDescriptor(Size win_size = Size(64, 128),
                  Size block_size = Size(16, 16),
                  Size block_stride = Size(8, 8), Size cell_size = Size(8, 8),
                  int nbins = 9, double win_sigma = DEFAULT_WIN_SIGMA,
                  double threshold_L2hys = 0.2, bool gamma_correction = true,
                  int nlevels = DEFAULT_NLEVELS);
    // 返回分类器的数目
    size_t getDescriptorSize() const;
    // 返回块区域直方图尺寸
    size_t getBlockHistogramSize() const;
    // 集系数的线性支持向量机分类器
    void setSVMDetector(const vector<float>& detector);
    // 返回系数检测分类器训练的人(默认窗口大小)
    static vector<float> getDefaultPeopleDetector();
    // 返回系数检测分类器训练的人
    static vector<float> getPeopleDetector48x96();
    static vector<float> getPeopleDetector64x128();
    // 执行对象检测单尺度窗口
    void detect(const oclMat& img, vector<Point>& found_locations,
                double hit_threshold = 0, Size win_stride = Size(),
                Size padding = Size());
    // 执行对象检测多尺度窗口
    void detectMultiScale(const oclMat& img, vector<Rect>&
                          found_locations, double hit_threshold = 0,
                          Size win_stride = Size(), Size padding = Size(),
                          double scale0 = 1.05, int group_threshold = 2);
    // 返回块描述符计算整个图像
    void getDescriptors(const oclMat& img, Size win_stride,
                        Feature Detection And Description
                        oclMat& descriptors,
                        int descr_format = DESCR_FORMAT_COL_BY_COL);
    // 检测窗口尺寸，块尺寸，窗口步尺寸，胞尺寸，
    // 直方图bins，L2范数阈值,gamma变换标志，检测窗增加最大数
    Size win_size;
    Size block_size;
    Size block_stride;
    Size cell_size;
    int nbins;
    double win_sigma;
    double threshold_L2hys;
    bool gamma_correction;
    int nlevels;
}
