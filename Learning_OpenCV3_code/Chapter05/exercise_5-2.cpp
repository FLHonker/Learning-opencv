// Create a two-dimensional matrix with three channels of type byte with data size
// 100 × 100, and set all the values to 0 . Use the cv::Mat element access functions to
// modify the pixels. Draw a green rectangle between (20, 5) and (40, 20).
//
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat m2(100, 100, CV_8UC3, Scalar(0));   //100 x 100, 32bit, 3 channals, filled by 0.
    for(int i=0; i < m2.rows; ++i)
    {
        for(int j=0; j < m2.cols; ++j)
        {
            if(j>=20 && i>=5 && j<=40 && i>=20)
            {
                m2.at<Vec3b>(i,j)[0] = 0;    //b lue
                m2.at<Vec3b>(i,j)[1] = 255;  //g reen
                m2.at<Vec3b>(i,j)[2] = 0;    //r ed
            }
        }
    }
    std::cout << m2 << std::endl;
    imshow("Rect matrix", m2);
    waitKey(0);

    return 0;
}
