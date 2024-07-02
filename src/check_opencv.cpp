#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;

int main()
{
    std::string image_path = "data/eyes.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);
    std::cout << "everything is working fine" << std::endl;
    return 0;
}