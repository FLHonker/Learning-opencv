//Combining the pyramid down operator (twice) and the Canny
//subroutine in a simple image pipeline
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat img_rgb, img_gray, img_pyr, img_pyr2, img_cny;

    namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
    namedWindow("Canny image", cv::WINDOW_AUTOSIZE);
    
    img_rgb = imread(argv[1]);

    cvtColor(img_rgb, img_gray, cv::COLOR_BGR2GRAY);
    imshow("Gray image", img_gray);
    pyrDown(img_gray, img_pyr);
    pyrDown(img_pyr, img_pyr2);
    Canny(img_pyr2, img_cny, 10, 100, 3, true);

    // do whatever with img_cny
    imshow("Canny image", img_cny);

    waitKey(0);

    return 0;
}
