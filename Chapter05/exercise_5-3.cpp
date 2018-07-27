// Create a three-channel RGB image of size 100 × 100. Clear it. Use pointer 
// arithmetic to draw a green square between (20, 5) and (40, 20).
//
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main(int arc, char** argv)
{
    Mat m3(100,100,CV_8UC3,Scalar(0));
    for(int i=0; i < m3.rows; ++i)
    {
        uchar* outData = m3.ptr<uchar>(i);
        for(int j=0; j < m3.cols; ++j)
        {
            if(i>=5 && i<=20 && j>=20 && j<=40)
                outData[j*3+1] = 255;   // green
        }
    }
    imshow("exercise3", m3);
    waitKey(0);

    return 0;
}
