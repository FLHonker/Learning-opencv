// C++ source file
/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: 9-2_videostab.cpp
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: 视频稳像操作
# @Create Time: 2018-11-18 10:53:16
# @Last Modified: 2018-11-18 10:53:16
***********************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/videostab.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::videostab;

// 视频稳定输出
void videoOuput(Ptr<IFrameSource> stabFrames, string outputPath)
{
    VideoWriter writer;
    Mat stabFrame;
    int nframes = 0;
    // 设置输出帧率
    double outputFps = 25;
    // 遍历搜索视频帧
    while(!(stabFrame = stabFrames->nextFrame()).empty())
    {
        nframes++;
        // 输出视频
        if(!outputPath.empty())
        {
            if(!writer.isOpened())
                writer.open(outputPath, VideoWriter::fourcc('X','V','I','D'), outputFps, stabFrame.size());
            writer << stabFrame;
        }
        imshow("stabFrame", stabFrame);
        // ESC键退出
        char key = static_cast<char>(waitKey(3000));
        if(key == 27)
        {
            cout << endl;
            break;
        }
    }
    cout << "nFrames:" << nframes <<endl;
    cout << "finished!" <<endl;
}

void cacStabVideo(Ptr<IFrameSource> stabFrames, string inputPath, string outputPath)
{
    try {
        Ptr<VideoFileSource> srcVideo = makePtr<VideoFileSource>(inputPath);
        cout << "frame count:" << srcVideo->count() <<endl;
        // 运动估计
        double estPara = 0.1;
        Ptr<MotionEstimatorRansacL2> est = makePtr<MotionEstimatorRansacL2>(MM_AFFINE);
        // Ransac 参数设置
        RansacParams ransac = est->ransacParams();
        ransac.size = 3;
        ransac.thresh = 5;
        ransac.eps = 0.5;
        // Ransac 计算
        est->setRansacParams(ransac);
        est->setMinInlierRatio(estPara);
        // Fast特征检测
        Ptr<FastFeatureDetector> feature_detector = FastFeatureDetector::create();
        // 运动估计关键点匹配
        Ptr<KeypointBasedMotionEstimator> motionEstBuilder = makePtr<KeypointBasedMotionEstimator>(est);
        // 设置特征检测器
        motionEstBuilder->setDetector(feature_detector);
        Ptr<IOutlierRejector> outlierRejector = makePtr<NullOutlierRejector>();
        motionEstBuilder->setOutlierRejector(outlierRejector);
        // 视频稳定
        StabilizerBase *stabilizer = 0;
        // 两次稳定过滤
        bool isTwoPass = 1;
        int radius_pass = 15;
        if(isTwoPass)
        {
            // 两级稳定运动估计
            bool est_trim = true;
            TwoPassStabilizer *twoPassStabilizer = new TwoPassStabilizer();
            twoPassStabilizer->setEstimateTrimRatio(est_trim);
            twoPassStabilizer->setMotionStabilizer(makePtr<GaussianMotionFilter>(radius_pass));
            stabilizer = twoPassStabilizer;
        }
        else
        {
            // 单级运动估计过滤
            OnePassStabilizer *onePassStabilizer = new OnePassStabilizer();
            onePassStabilizer->setMotionFilter(makePtr<GaussianMotionFilter>(radius_pass));
            stabilizer = onePassStabilizer;
        }
        // 运动稳定类参数
        int radius = 15;
        double trim_ratio = 0.1;
        bool incl_constr = false;
        // 运动估计视频稳像
        stabilizer->setFrameSource(srcVideo);
        stabilizer->setMotionEstimator(motionEstBuilder);
        stabilizer->setRadius(radius);
        stabilizer->setTrimRatio(trim_ratio);
        stabilizer->setCorrectionForInclusion(incl_constr);
        stabilizer->setBorderMode(BORDER_REPLICATE);
        // 视频帧重置
        stabFrames.reset(dynamic_cast<IFrameSource*>(stabilizer));
        // 视频稳像输出
        videoOuput(stabFrames, outputPath);
    } catch(const exception &e) {
        cout << "error: " << e.what() <<endl;
        stabFrames.release();
    }
}

int main()
{
    Ptr<IFrameSource> stabFrames;
    // 输入/输出视频设备
    string inputPath = "../../imgdata/tree.avi";
    string outputPath = "../../imgdata/resStab.avi";
    // 视频稳像
    cacStabVideo(stabFrames, inputPath, outputPath);
    stabFrames.release();
    return 0;
}
