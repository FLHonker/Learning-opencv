// load and display a vedio
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    namedWindow("display video", cv::WINDOW_AUTOSIZE);
    VideoCapture cap;
    cap.open(String(argv[1]));

    Mat frame;
    while(true)
    {
        cap >> frame;
        if (frame.empty())
            return -1;
        imshow("display video", frame);
        if (waitKey(33) >= 0)
            break;
    }
    return 0;
}
