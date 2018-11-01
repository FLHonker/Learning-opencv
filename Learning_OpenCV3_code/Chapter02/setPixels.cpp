//Getting and setting pixels in Pyramid_com_Canny example2-8
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

    // pixel opt
    int x = 16, y = 32;
    Vec3b intensity = img_rgb.at<Vec3b>(y,x);
    // Note: We could write img_rgb.at<Vec3b>(x,y)[0];
    
    // get BGR
    uchar blue = intensity[0];
    uchar green = intensity[1];
    uchar red = intensity[2];

    // output
    std::cout << "At (x,y) = (" << x << "," << y <<"):(blue, green, red) = ("
        << (unsigned int)blue << "," << (unsigned int)green << "," 
        << (unsigned int)red << ")" << std::endl;

    std::cout << "Gray pixel there is: " << (unsigned int)img_gray.at<uchar>(y,x) << std::endl;

    x /= 4; y /= 4;
    std::cout << "Pyremid2 pixel there is: " << (unsigned int)img_pyr2.at<uchar>(y,x) << std::endl;

    img_cny.at<uchar>(x,y) = 128;   //Set the Canny pixel there to 128;

    imshow("Pyrmid 128 pixels", img_cny);

    waitKey(0);

    return 0;
}
