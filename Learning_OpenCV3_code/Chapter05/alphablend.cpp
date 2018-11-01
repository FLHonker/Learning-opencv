// Complete program to alpha-blend the ROI starting at (0,0) in src2 with
// the ROI starting at (x,y) in src1.
// Usage: alphablend <imageA> <image B> <x> <y> <width> <height> <alpha> <beta>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
    Mat src1 = imread(argv[1],1);
    Mat src2 = imread(argv[2],1);

    if(argc == 9 && !src1.empty() && !src2.empty())
    {
        int x = atoi(argv[3]);
        int y = atoi(argv[4]);
        int w = atoi(argv[5]);
        int h = atoi(argv[6]);
        double alpha = (double)atof(argv[7]);
        double beta  = (double)atof(argv[8]);

        Mat roi1(src1, Rect(x,y,w,h));
        Mat roi2(src2, Rect(x,y,w,h));

        namedWindow("image src1", 1);
        namedWindow("image src2", 2);
        imshow("image src1", src1);
        imshow("image src2", src2);
        
        addWeighted(roi1, alpha, roi2, beta, 0.0, roi1);
        imshow("roi1", roi1);
        std::cout << "roi1:" << std::endl << roi1 << std::endl;

        waitKey(0);
    }
    else if (argc != 9)
    {
        std::cout << "Usage: alphablend <imageA> <image B> <x> <y> <width> <height> <alpha> <beta>" << std::endl;
    }
    return 0;
}
