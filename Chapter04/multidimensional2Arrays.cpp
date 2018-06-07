//Example 4-1. Summation of a multidimensional array, done plane by plane.
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    const int n_mat_size = 5;
    const int n_mat_sz[] = {n_mat_size, n_mat_size, n_mat_size};
    Mat n_mat0(3, n_mat_sz, CV_32FC1);
    Mat n_mat1(3, n_mat_sz, CV_32FC1);

    cv::RNG rng;
    rng.fill(n_mat0, cv::RNG::UNIFORM, 0.f, 1.f);
    rng.fill(n_mat1, cv::RNG::UNIFORM, 0.f, 1.f);

    const Mat* arrays[] = {&n_mat0, &n_mat1, 0};
    Mat my_planes[2];
    NAryMatIterator it(arrays, my_planes);

    //On each iteration, it.planes[i] will be the current plane of the
    //i-th array from 'arrays'
    float s = 0.f;  // Total sum over all planes
    int n = 0;      // Total number of planes
    for (int p = 0; p < it.nplanes; p++, ++it)
    {
        s += cv::sum(it.planes[0])[0];
        s += cv::sum(it.planes[1])[0];
        n++;
        std::cout << "planes:" << std::endl << *it.planes << std::endl;
        std::cout << "planes:" << std::endl << *(++it.planes) << std::endl;
    }
    std::cout << "n = " << n << ", s = " << s << std::endl;

    return 0;
}
