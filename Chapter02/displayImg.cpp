#include <opencv2/opencv.hpp>   //include file for every supported OpenCV function

int main(int argc, char** argv)
{
    cv::Mat img = cv::imread(argv[1], -1);
    if(img.empty())
        return -1;
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", img);
    cv::waitKey( 0 );
    cv::destroyWindow("Display Image");
    return 0;
}
