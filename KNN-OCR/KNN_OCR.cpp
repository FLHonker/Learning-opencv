// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: KNN-OCR.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: K邻近算法, 识别手写数字
# @Create Time: 2018-07-19 12:34:37
# @Last Modified: 2018-07-19 12:34:37
***********************************************/
/*
 * 图片大小为1000*2000,有0-9的10个数字，每5行为一个数字，总共50行，共有5000个手写数字。
 * 在opencv3.0版本中，图片存放位置为 `/opencv/sources/samples/data/digits.png`
 * 我们首先要做的，就是把这5000个手写数字，一个个截取出来，每个数字块大小为20*20。
 * 直接将每个小图块进行序列化，因此最终得到一个5000*400的特征矩阵。样本数为5000，维度为400维。取其中前3000个样本进行训练。
 * 注意：截取的时候，是按列截取。不然取前3000个样本进行训练就会出现后几个数字训练不到。
 */
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage: ./" << argv[0] << " image" << endl;
        return -1;
    }
    cout << "starting..." <<endl;
    double stime = (double)cvGetTickCount();
    Mat img = imread(argv[1], 0);
    int boot = 20;
    int m = img.rows / boot, n = img.cols / boot;
    Mat data, labels;   //特征矩阵，data和labels分别存放

    //截取数据的时候要按列截取
    for(int i = 0; i < n; i++)
    {
        int colNum = i * boot;  //列上的偏移量
        for(int j = 0; j < m; j++)
        {
            int rowNum = j * boot; //行上的偏移量
            //截取20*20的小块
            Mat tmp;
            img(Range(rowNum, rowNum + boot), Range(colNum, colNum + boot)).copyTo(tmp);
            data.push_back(tmp.reshape(0, 1));  //序列化后放入特征矩阵
            labels.push_back((int)j / 5);       //对应的标注
        }
    }
    data.convertTo(data, CV_32F);  //uchar型转换为cv_32f
    int sampleNum = data.rows;
    int trainNum = 3000;

    Mat trainData, trainLabel;
    trainData = data(Range(0,trainNum), Range::all());
    trainLabel = labels(Range(0,trainNum), Range::all());
   
    //使用KNN算法
    int k = 5;
    Ptr<TrainData> tData = TrainData::create(trainData, ROW_SAMPLE, trainLabel);   //ROW_SAMPLE表示一行一个样本
    Ptr<KNearest> model = KNearest::create();
    model->setDefaultK(k);
    model->setIsClassifier(true);
    model->train(tData);

    //预测分类
    double train_hr = 0, test_hr = 0;
    Mat response;
    for(int i = 0; i < sampleNum; i++)
    {
        Mat sample = data.row(i);
        float r = model->predict(sample);  //对所有行进行预测
        //预测结果与原结果相比，相等为1，不等为0
        r = abs(r - labels.at<int>(i));
        if(r <= FLT_EPSILON)  //FLT_EPSILON表示最小的float浮点数，小于它近乎为0
            r = 1.f;
        else
            r = 0.f;
        if(i < trainNum)
            train_hr = train_hr + r;  //累积正确数
        else
            test_hr = test_hr + r;
    }
    
    stime = cvGetTickCount() - stime;
    cout << "Finished." <<endl;
    cout << "KNN模型在训练集上的准确率为：" << train_hr / trainNum * 100 << "%, 在测试集上的准确率为：" << test_hr / (data.rows - trainNum)*100 << "%" << endl;
    cout << "耗时：" << stime / (cvGetTickFrequency() * 1000000) << "s" <<endl;  //秒
    waitKey(0);

    return 0;
}
