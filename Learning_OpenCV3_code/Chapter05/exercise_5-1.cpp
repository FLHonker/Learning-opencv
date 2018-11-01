// exercise_5-1:
// Create a two-dimensional matrix with three channels of type byte with
// data size 100 × 100. Set all the values to 0 .
//   a. Draw a circle in the matrix using void cv::circle(InputOutputArray img,
// cv::point center, intradius, cv::Scalar color, int thickness=1,
// int line_type=8, int shift=0) .
//   b. Display this image using methods described in Chapter 2.
//
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    Mat m(100, 100, CV_32FC3, cv::Scalar(0,0,0));   //100 x 100, 32bit, 3 channals, filled by 0.
    Point2i pt(m.rows/2, m.cols/2);
    Scalar color(0,0,255);  //red
    circle(m, pt, 10, color);
    std::cout << "matrix:" << std::endl << m << std::endl;
    imshow("100 x 100 matrix", m);
    waitKey(0);

    return 0;
}
