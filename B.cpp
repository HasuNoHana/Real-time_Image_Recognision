#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

void read_from_file(cv::Mat &src, int argc, char** argv) {
    cv::String imageName("stuff.jpg"); // by default
    if (argc > 1) {
        imageName = argv[1];
    }
    src = cv::imread( cv::samples::findFile( imageName ), cv::IMREAD_COLOR ); // Load an image
    if (src.empty()) {
        throw std::runtime_error("Cannot read the image");
    }
}

void threshold(cv::Mat &src, cv::Mat &dst) {
    cv::Mat src_gray;
    cv::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY ); // Convert the image to Gray
    src_gray.copyTo(dst);
    int threshold_type = 0;                 // 0 = binary, 1 = binary inverted, 2 = truncated, 3 = to zero, 4 = to zero inverted
    int threshold_value = 230;
    int const max_binary_value = 255;

    cv::threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );
}

void locate(cv::Mat &img, int &x, int &y, cv::Mat &src) {
    int window_row_n = 60, window_col_n = 60;
    int step = 20, sub_step = 3;
    int count, max = 0;
    cv::Mat sub_img;
    for (int row = 0; row <= img.rows - window_row_n; row += step) {
        for (int col = 0; col <= img.cols - window_col_n; col += step) {
            cv::Rect window(col, row, window_row_n, window_col_n);
            sub_img = img(window);
            count = 0;
            for (int k = 0; k <= sub_img.rows; k += sub_step) {
                for (int l = 0; l <= sub_img.cols; l += sub_step) {
                    if (sub_img.at<int>(cv::Point(l,k)) == 255)
                        count++;
                }
            }
            // std::cout << count << ' ';
            if (count >= max) {
                x = col + window_col_n/2;
                y = row + window_row_n/2;
                max = count;
            }
        }
        // std::cout << std::endl;
    }
    cv::circle(src, cv::Point(x, y), 30, cv::Scalar(100), 1, cv::LINE_AA);

}

int main(int argc, char** argv) {
    
    cv::Mat src;
    try {
        read_from_file(src, argc, argv);
    } catch (std::runtime_error e) {
        return -1;
    }

    cv::Mat dst;
    threshold(src, dst);

    int x, y;
    locate(dst, x, y, src);

    const char* window_name = "Threshold Demo";
    cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE ); // Create a window to display results
    cv::imshow( window_name, src );
    cv::waitKey();
    cv::destroyWindow(window_name);
}