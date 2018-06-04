#include <opencv2/opencv.hpp>   //include file for every supported OpenCV function

using namespace cv;

int main(int argc, char** argv)
{
    Mat img = imread(argv[1], -1);
    if(img.empty())
        return -1;
    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    imshow("Display Image", img);
    waitKey( 0 );
    destroyWindow("Display Image");
    return 0;
}
