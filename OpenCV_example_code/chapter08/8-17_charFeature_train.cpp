// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 8-17_charFeature_train.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 字符特征训练
# @Create Time: 2018-11-01 16:08:18
# @Last Modified: 2018-11-01 16:08:18
***********************************************/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 字符的特征性向量训练
// 特征提取函数：输入图像，输出特征向量
typedef Mat (*FEATURE_EXTRACTOR)(Mat);
// Label生成函数：输入子文件夹名称，输出label
typedef double (*LABEL_MAKER)(string);
// 生成训练文件
void make_train_file(string src_dir, string dst, 
                     FEATURE_EXTRACTOR fea_extractor, LABEL_MAKER label_maker, 
                     int maxnum_per_class, bool shuffle)
{
    ofstream fs(dst.c_str());
    vector<string> sub_dirs;
    // 读入多个待训练图像源
    src_dir = TrimPath(src_dir);
    FindSubDirs(src_dir, sub_dirs);
    for(size_t i = 0; i < sub_dirs.size(); ++i)
    {
        string sub = src_dir + "/" + sub_dirs[i];
        vector<string> img_paths;
        FindImages(sub, img_paths);
        // 随机选取
        if(shuffle)
        {
            random_shuffle(img_paths.begin(), img_paths.end());
        }
        if(img_paths.size() > maxnum_per_class)
        {
            img_paths.erase(img_paths.begin() + maxnum_per_class, img_paths.end());
        }
        for(size_t j = 0; j < img_paths.size(); ++j)
        {
            // 训练文件每行的结构
            // 实数的类标签——维数：该维数据
            fs << label_maker(sub_dirs[i]) << " ";
            // cout << sub_dirs[i] << endl;
            Mat pic = imread(img_paths[j]);
            // 特征提取
            Mat fea = fea_extractor(pic);
            float* dptr = (float*)fea.data;
            for(size_t k = 0; k < fea.size(); ++k, ++dptr)
            {
                fs << (k+1) << ":" << *dptr << " ";
            }
            fs << "\n";
        }
    }
    fs.close();
}
