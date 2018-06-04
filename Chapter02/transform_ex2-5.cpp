// Loading and then smoothing an image before it 
// is displayed on the screen.
#include <opencv2/opencv.hpp>

using namespace cv;

void smooth(const Mat &image)
{
    //Create some windows to how the input
    //and output images in.
    namedWindow("transform-in", cv::WINDOW_AUTOSIZE);
    namedWindow("transform-out", cv::WINDOW_AUTOSIZE);

    //Create a window to show the input image
    imshow("transform-in", image);

    //Create an image to hold the smoothed output
    Mat out;

    //Do the smoothing
    //Note:Could use GaussianBlur(), blur(), medianBlur() or bilateralFilter()
    GaussianBlur(image, out, cv::Size(5, 5), 3, 3);
    GaussianBlur(  out, out, cv::Size(5, 5), 3, 3);

    //Show the smoothed image in the output window
    imshow("transform-out", out);

    //Wait for the user to hit a key, windows will self destruct.
    waitKey(0);
}

int main(int argc, char** argv)
{
    Mat imagein = imread(argv[1], -1);
    smooth(imagein);

    return 0;
}

