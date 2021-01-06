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

void locate(cv::Mat &img, int &x, int &y) {
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
    // cv::circle(src, cv::Point(x, y), 30, cv::Scalar(100), 1, cv::LINE_AA);  // draw a circle to point at the overexposed area
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
    
    // open message queue
	int id_kolejki = msgget(KLUCZ_KOLEJKA, 0);
	if(id_kolejki==-1){
		std::cerr << "Error while opening message queue" << std::endl;
		exit(1);
	}

	// open shared memory 
	int id_pamieci = shmget(KLUCZ_PAMIEC, 0, 0);
	if(id_pamieci==-1){
		std::cerr << "Error while opening shared memory" << std::endl;
		exit(1);
	}

    // attaching shared memory segment
    void *shared_frame = (void *)(cv::Mat *)shmat(id_pamieci, NULL, 0);
    if (shared_frame == (void *) -1) {
        std::cerr << "Error while attaching shared memory segment" << std::endl;
        exit(1);
    }

    cv::Mat src;
    cv::Mat dst;
    int x, y;

    bufmsg buf;

    // ready to receive the first frame
    buf.mtype = 2;
    if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
        std::cerr << "Error while sending message to A" << std::endl;
        exit(1);
    }
    std::cerr << "message to A sent" << std::endl;

    int i = 0;
    while (true) {
        if (msgrcv(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 1, 0) == -1) {
            std::cerr << "Error while receiving message from A" << std::endl;
            exit(1);
        }
        std::cerr << "message from A received" << std::endl;

        // using shared memory
        std::cerr << "trying to use shared memory" << std::endl;

        src = ((cv::Mat) *(cv::Mat *)shared_frame);
        std::cerr << "assigned memory to Mat" << std::endl;
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY); // Convert the image to Gray
        std::cerr << "converted image to gray" << std::endl;
        locate(dst, x, y);
        std::cerr << "located controller" << std::endl;

        //finished using shared memory
        buf.mtype = 2;
        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to A" << std::endl;
            exit(1);
        }
        std::cerr << "message to A sent" << std::endl;

        buf.mtype = 3;
        strncpy(buf.mtext, format_message(x, y, dst.cols, dst.rows).c_str(), ROZMIAR_KOMUNIKATU);

        if (msgsnd(id_kolejki, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            std::cerr << "Error while sending message to C" << std::endl;
            exit(1);
        }
        i++;
        if (i >= 6) break;
    }
    if (shmdt(shared_frame) == (uchar) -1) {
        std::cerr << "Error while closing shared memory" << std::endl;
    }
}