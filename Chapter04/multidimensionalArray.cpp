//Example 4-1. Summation of a multidimensional array, done plane by plane.
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    const int n_mat_size = 5;
    const int n_mat_sz[] = {n_mat_size, n_mat_size, n_mat_size};
    Mat n_mat(3, n_mat_sz, CV_32FC1);

    cv::RNG rng;
    rng.fill(n_mat, cv::RNG::UNIFORM, 0.f, 1.f);

    const Mat* arrays[] = {&n_mat, 0};
    Mat my_planes[1];
    NAryMatIterator it(arrays, my_planes);

    //On each iteration, it.planes[i] will be the current plane of the
    //i-th array from 'arrays'
    float s = 0.f;  // Total sum over all planes
    int n = 0;      // Total number of planes
    for (int p = 0; p < it.nplanes; p++, ++it)
    {
        s += cv::sum(it.planes[0])[0];   //通道0 (只有一个通道)
        n++;
        std::cout << "planes:" << std::endl << *it.planes << std::endl;
    }
    std::cout << "n = " << n << ", s = " << s << std::endl;

    return 0;
}
