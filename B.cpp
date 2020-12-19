#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv ) {

    String imageName("stuff.jpg"); // by default
    if (argc > 1) {
        imageName = argv[1];
    }
    Mat src = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Load an image
    if (src.empty()) {
        std::cout << "Cannot read the image: " << imageName << std::endl;
        return -1;
    }

    Mat src_gray;
    cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to Gray

    Mat dst;

    int threshold_type = 0;                 // 0 = binary, 1 = binary inverted, 2 = truncated, 3 = to zero, 4 = to zero inverted
    int threshold_value = 230;
    int const max_binary_value = 255;

    threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );

    const char* window_name = "Threshold Demo";
    namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
    imshow( window_name, dst );
    waitKey();
}