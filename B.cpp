#include <iostream>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define KLUCZ_KOLEJKA 12345
#define KLUCZ_PAMIEC 12346

#define ROZMIAR_KOMUNIKATU 50
#define ROZMIAR_PAMIECI 100

struct bufmsg{
	long mtype; // 1: A->B, 2: B->A, 3: B->C
	char mtext[ROZMIAR_KOMUNIKATU];
};

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
            if (count > max) {                          // there are more white pixels than ever before
                x = col + window_col_n/2;
                y = row + window_row_n/2;
                max = count;
            }
        }
    }
    cv::circle(src, cv::Point(x, y), 30, cv::Scalar(100), 1, cv::LINE_AA);  // draw a circle to point at the overexposed area
}

std::string format_message(int &x, int &y, int &Xsize, int &Ysize) {
    std::stringstream message;
    message << std::setw(4) << x;
    message << std::setw(4) << y;
    message << std::setw(4) << Xsize;
    message << std::setw(4) << Ysize;
    return message.str();
}

int main(int argc, char** argv) {
    
    //otwarcie kolejki
	int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
	if(id_kolejki==-1){
		std::cout<<"Blad otwarcia kolejki\n";
		return 1;
	}

	//otwarcie pamięci współdzielonej
	int id_pamieci = shmget(KLUCZ_PAMIEC, 0, 0);
	if(id_pamieci==-1){
		std::cout<<"Blad otwarcia pamieci\n";
		return 1;
	}

    cv::Mat src;
    cv::Mat dst;
    int x, y;

    bufmsg buf;

    int i = 0;
    while (true) {
        buf.mtype = 2;
        strncpy(buf.mtext, "w", ROZMIAR_KOMUNIKATU);
        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to A" << std::endl;
            return 1;
        }
        std::cerr << "message to A sent" << std::endl;
        if (msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 1, 0) == -1) {
            std::cerr << "Error while receiving message from A" << std::endl;
            return 1;
        }
        std::cerr << "message from A received" << std::endl;
        // shared memory handling
        read_from_file(src, argc, argv); //mock, to be deleted


        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY); // Convert the image to Gray
        locate(dst, x, y, src);

        buf.mtype = 3;
        strncpy(buf.mtext, format_message(x, y, dst.cols, dst.rows).c_str(), ROZMIAR_KOMUNIKATU);

        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to C" << std::endl;
            return 1;
        }
        i++;
        if (i >= 6) break;
    }
//
//     const char* window_name = "Controller location";
//     cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE ); // Create a window to display results
//     cv::imshow( window_name, src );
//
//     cv::waitKey();
//     cv::destroyWindow(window_name);
}