//The Canny edge detector writes its output to a single-channel (grayscale) image
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat img_rgb, img_gray, img_cny;
    namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
    namedWindow("Canny image", cv::WINDOW_AUTOSIZE);

    img_rgb = imread(argv[1]);

    cvtColor(img_rgb, img_gray, cv::COLOR_BGR2GRAY);
    imshow("Gray image", img_gray);

    Canny(img_gray, img_cny, 10, 100, 3, true);
    imshow("Canny image", img_cny);

    waitKey(0);

    return 0;
}
