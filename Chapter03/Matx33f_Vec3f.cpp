//a. Using the cv::Mat33f and cv::Vec3f objects (respectively), create a 3 × 3
//matrix and 3-row vector.
//b. Can you multiply them together directly? If not, why not?
#include <opencv2/opencv.hpp>

int main()
{
    cv::Matx33f m33f = cv::Matx33f::eye();
    cv::Vec3f v3f(1,2,3);
    std::cout << "m33f:" << std::endl << m33f << std::endl;
    std::cout << "v3f:" << v3f << std::endl;
    std::cout << "m33f * v3f = " << m33f*v3f << std::endl;
    std::cout << std::endl;

    cv::Matx<float, 3, 3> mxtpl(1,0,0,0,1,0,0,0,1);
    cv::Vec<float, 3> vtpl(1,1,1);
    std::cout << "mxtpl:" << std::endl << mxtpl << std::endl;
    std::cout << "vtpl:" << vtpl << std::endl;
    std::cout << "mxtpl * vtpl = " << mxtpl*vtpl << std::endl;

    return 0;
}
