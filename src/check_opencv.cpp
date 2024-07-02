#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    std::string image_path = "path/to/image";
    Mat img = imread(image_path, IMREAD_COLOR);
    return 0;
}