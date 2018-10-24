// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-2_DescriptorExtractor.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: DescriptorExtractor类
# @Create Time: 2018-10-21 21:29:34
# @Last Modified: 2018-10-21 21:29:34
***********************************************/
// 特征描述类，计算出特征描述子
class CV_EXPORTS DescriptorExtractor
{
public:
    virtual ~DescriptorExtractor();
    // 计算单个图像的关键点描述子
    void compute(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors) const;
    // 计算多个图像的关键点描述子
    void compute(const vector<Mat>& images, vector<vector<KeyPoint>>& keypoints, 
                 vector<Mat>& descriptors) const;
    // 读取文件中特征描述的子对象
    virtual void read(const FileNode&);
    // 写文件特征描述子提取的对象
    virtual void write(FileStorage&) const;
    // 获取描述子尺寸
    virtual int descriptorSize() const = 0;
    // 获取描述子类型
    virtual int descriptorType() const = 0;
    // 创建特征描述子对象
    static Ptr<DeacriptorExtractor> create(const string& deacriptorExtractorType);
protected:
    // ...
};
// 计算SIFT特征描述的子类
// classSiftDescriptorExtractor : public DeacriptorExtractor
