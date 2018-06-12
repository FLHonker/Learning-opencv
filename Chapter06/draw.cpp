// draw test: 
// circle, clipLine, elipLine, ellipse, ellipse2Poly, fillConvexPoly,
// line, rectangle, polyLines

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char **argv)
{
    Mat pic0 = imread(argv[1], -1);
    Mat pic1(pic0);
    Point pt(pic1.rows, pic1.cols);
    Scalar color(0,0,255);
    std::cout << "pic1 size = " << pic1.size << std::endl;
    std::cout << "point:" << std::endl << pt << std::endl;
    namedWindow("draw", cv::WINDOW_AUTOSIZE);
    // draw
    circle(pic1, pt, 60, color, 1, 8, 1.2);
    imshow("draw", pic1);
    waitKey(0);

    return 0;
}
