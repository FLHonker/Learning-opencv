// Example 2-4. Adding a trackbar slider to the basic viewer window for 
// moving around within the video file.

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace std;

int g_slider_position = 0;
int g_run = 1, g_dontset = 0;   //start out in single step mode
cv::VideoCapture g_cap;

void onTrackbarSlider(int pos, void *)
{
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}

int main(int argc, char** argv)
{
    cv::namedWindow("trackbar", cv::WINDOW_AUTOSIZE);
    g_cap.open(string(argv[1]));
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    int tmpw = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int tmph = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "Video has " << frames << " frames of dimensios("
        << tmpw << ", " << tmph << ")." << endl;
    cv:: createTrackbar("Position", "trackbar", &g_slider_position, frames, onTrackbarSlider);
    cv::Mat frame;
    while(true)
    {
        if(g_run != 0)
        {
            g_cap >> frame;
            if (frame.empty())
                break;
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            g_dontset = 1;

            cv::setTrackbarPos("Position", "trackbar", current_pos);
            cv::imshow("trackbar", frame);

            g_run -= 1;
        }
        char c = (char)cv::waitKey(100);
        if (c == 's')   //single step
        {
            g_run = 1;
            cout << "Single step, run = " << g_run << endl;
        }else if (c == 'r'){    //run mode
            g_run = -1;
            cout << "Run mode, run = " << g_run << endl;
        }else if (c == 27) {
            break;
        }
    }
    return 0;
}
