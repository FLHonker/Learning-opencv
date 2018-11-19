// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-4_HDR.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 高动态范围(High Dynamic Range)
# @Create Time: 2018-11-19 13:17:19
# @Last Modified: 2018-11-19 13:17:19
***********************************************/
#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void loadExposureSeq(string path, vector<Mat>& images, vector<float>& times)
{
    // 获取当前路径
    path = path + string("/");
    // 获取图像相机的曝光信息
    ifstream list_file((path + "list.txt").c_str());
    string name;
    float val;
    // 遍历完成图像源信息输入
    while(list_file >> name >> val)
    {
        Mat img = imread(path + name);
        images.push_back(img);
        times.push_back(1.0 / val);
    }
    list_file.close();
}

/**
 * @brief 
 *
 * @param argc
 * @param argv
 *
 * @return 0
 */
int main(int argc, char** argv)
{
    vector<Mat> images;
    vector<float> times;
    // 获取图像序列
    string path = "../../imgdata/HDR/Memorial_SourceImages";
    loadExposureSeq(path, images, times);
    // 创建Calibrate对象
    Mat response;
    Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
    // 估计相机响应，校准算法来估计逆CRF所有的256像素值
    calibrate->process(images, response, times);
    // 构建HDR图像
    Mat hdr;
    Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
    merge_debevec->process(images, hdr, times, response);
    // 图像映射，利用双边滤波Gamma变换
    Mat ldr;
    Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
    tonemap->process(hdr, ldr);
    // 执行曝光融合
    Mat fusion;
    Ptr<MergeMertens> merge_mertens = createMergeMertens();
    merge_mertens->process(images, fusion);
    imshow("hdr", hdr);
    // 结果输出
    imwrite("fusion.png", fusion * 255);
    imwrite("ldr.png", ldr * 255);
    imwrite("hdr.hdr", hdr);
    waitKey(0);
    return 0;
}
