//Using cv::pyrDown() to create a new image that is half the width 
//and height of the input image.  
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat img1, img2;

    namedWindow("image1", cv::WINDOW_AUTOSIZE);
    namedWindow("image2", cv::WINDOW_AUTOSIZE);

    img1 = imread(argv[1]);
    imshow("image1", img1);

    pyrDown(img1, img2);
    imshow("image2", img2);

    waitKey(0);

    return 0;
}
