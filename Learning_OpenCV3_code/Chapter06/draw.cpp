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
    Point2i pt1(pic1.rows/3, pic1.cols/3), pt2(pic1.rows/2, pic1.cols/2);
    Scalar color(0,0,255);
    Size ellipse_sz(60, 40);
    String text("你好，opencv3!");
    std::cout << "pic1 size = " << pic1.size << std::endl;
    std::cout << "point:" << std::endl << pt1 << std::endl;
    namedWindow("draw", cv::WINDOW_AUTOSIZE);
    // draw
    circle(pic1, pt1, 60, color, 1, 8, 1.2);
    ellipse(pic1, pt2, ellipse_sz, 60, 0, 360, color);
    // 中文乱码，cv字体不支持中文
    putText(pic1, text, pt1, cv::FONT_HERSHEY_COMPLEX, 1.0, color);
    imshow("draw", pic1);
    waitKey(0);

    return 0;
}
