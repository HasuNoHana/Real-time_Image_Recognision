#include <iostream>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fcntl.h>  
#include <sys/stat.h>
#include <mqueue.h>

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

void locate(cv::Mat &img, int &x, int &y, cv::Mat &src) {
    int window_row_n = 60, window_col_n = 60;
    int step = 30, sub_step = 5;
    int count, max = 0;
    int threshold_value = 242;

    for (int row = 0; row < img.rows - window_row_n; row += step) {         // move sliding window
        for (int col = 0; col < img.cols - window_col_n; col += step) {     //
            count = 0;
            for (int k = 0; k < window_row_n; k += sub_step) {                          // probe sliding window
                for (int l = 0; l < window_col_n; l += sub_step) {                      //
                    if (img.at<uchar>(row + k, col + l) >= threshold_value) count++;    //
                }
            }
            // std::cout << count << ' ';
            if (count > max) {                          // there are more white pixels than ever before
                x = col + window_col_n/2;
                y = row + window_row_n/2;
                max = count;
            }
        }
        // std::cout << std::endl;
    }
    cv::circle(src, cv::Point(x, y), 30, cv::Scalar(100), 1, cv::LINE_AA);  // draw a circle to point at the overexposed area
}

int main(int argc, char** argv) {
    
    cv::Mat src;
    try {
        read_from_file(src, argc, argv);
    } catch (std::runtime_error e) {
        return -1;
    }

    cv::Mat dst;
    cv::cvtColor( src, dst, cv::COLOR_BGR2GRAY ); // Convert the image to Gray
    assert(dst.type() == CV_8U);

    int x, y;
    locate(dst, x, y, src);

    struct mq_attr attr;
    attr.mq_maxmsg = 16;
    attr.mq_msgsize = 16;
    attr.mq_flags = 0;

    mqd_t BC_queue = mq_open("/BC_queue", O_WRONLY | O_CREAT, 0664, &attr);
    // std::cout << EACCES << std::endl;
    // std::cout << EEXIST << std::endl;
    // std::cout << EINVAL << std::endl;
    // std::cout << EMFILE << std::endl;
    // std::cout << ENAMETOOLONG << std::endl;
    // std::cout << ENFILE << std::endl;
    // std::cout << ENOENT << std::endl;
    // std::cout << ENOMEM << std::endl;
    // std::cout << ENOSPC << std::endl;

    // std::cout << std::endl;
    // std::cout << errno << std::endl;

    std::stringstream message;
    message << std::setw(8) << x;
    message << std::setw(8) << y;
    const char *msg_ptr = message.str().c_str();
    
    mq_send(BC_queue, msg_ptr, 16, 1);        // queue name, message, length, priority

    // mq_unlink("/BC_queue");
    mq_close(BC_queue);

    const char* window_name = "Controller location";
    cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE ); // Create a window to display results
    cv::imshow( window_name, src );
    
    cv::waitKey();
    cv::destroyWindow(window_name);
}