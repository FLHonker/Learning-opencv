//The same object can load videos from a camera or a file.
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv)
{
    namedWindow("camera", cv::WINDOW_AUTOSIZE);
    VideoCapture cap;
    if (argc == 1)
    {
        cap.open(0);    //open the first camera
    }else {
        cap.open(argv[1]);
    }
    if (!cap.isOpened())
    {
        std::cerr << "Could't open capture." << std::endl;
        return -1;
    }

    // The following is the same as `displayVideo.cpp`
    Mat frame;
    while(true)
    {
        cap >> frame;
        imshow("camera", frame);
        if (waitKey(100) >= 0)
            break;
    }

    return 0;
}
