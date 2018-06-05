//A complete program to read in a color video and write out the 
//log-polar-transformed video.
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv)
{
    namedWindow("bgr frame", cv::WINDOW_AUTOSIZE);
    namedWindow("Log Polar", cv::WINDOW_AUTOSIZE);

    //Note:could capture from a camera by gving a camera id as an int.
    VideoCapture capture(argv[1]);

    double fps = capture.get(cv::CAP_PROP_FPS);
    cv::Size size(
        (int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT)
    );
    VideoWriter writer;
    writer.open(argv[2], CV_FOURCC('M','J','P','G'), fps, size);

    Mat logpolar_frame, bgr_frame;
    for(;;) 
    {
        capture >> bgr_frame;
        if (bgr_frame.empty())
            break;      //end of it done.
        imshow("bgr frame", bgr_frame);
        cv::logPolar(
            bgr_frame,          //input color frame
            logpolar_frame,     //output log-polar frame
            cv::Point2f(
                bgr_frame.cols/2,   // x
                bgr_frame.rows/2    // y
            ),
            40,                 //Magnitude(scale parameter)
            cv::WARP_FILL_OUTLIERS  //Fill outfiles with 'zero'
        );

        imshow("Log Polar", logpolar_frame);
        writer << logpolar_frame;
        
        if(waitKey(30) == 27)
            break;      //allow the user to break out.
    }
    capture.release();
    return 0;
}
